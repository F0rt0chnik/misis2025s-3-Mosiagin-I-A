#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Metrics {
    double IoU;
    int tp, fp, fn;
    double precision, recall, f1;

    void setValues(double iou, int tp, int fp, int fn) {
        this->IoU = iou;
        this->tp = tp;
        this->fp = fp;
        this->fn = fn;
        this->precision = (tp + fp > 0) ? static_cast<double>(tp) / (tp + fp) : 0.0;
        this->recall = (tp + fn > 0) ? static_cast<double>(tp) / (tp + fn) : 0.0;
        this->f1 = (precision + recall > 0) ? 2 * precision * recall / (precision + recall) : 0.0;
    }
};

std::vector<cv::RotatedRect> ParseGroundTruth(const json& data) {
    int tile_size = data["tile_size"].get<int>();

    std::vector<cv::RotatedRect> ground_truth_rects;

    for (const auto& tile : data["tiles"]) {
        const auto& elps = tile["elps_parameters"];
        const auto& coords = tile["pic_coordinates"];

        int abs_x = coords["col"].get<int>() * tile_size;
        int abs_y = coords["row"].get<int>() * tile_size;

        cv::Point center(
            elps["elps_x"].get<int>() + abs_x,
            elps["elps_y"].get<int>() + abs_y
        );
        float width = elps["elps_width"].get<float>() * 2;
        float height = elps["elps_height"].get<float>() * 2;
        float angle = elps["elps_angle"].get<float>();

        ground_truth_rects.emplace_back(center, cv::Size2f(width, height), angle);
    }

    return ground_truth_rects;
}

std::vector<cv::RotatedRect> ParsePredictions(const json& data) {
    std::vector<cv::RotatedRect> prediction_rects;

    for (const auto& prediction : data["detections"]) {
        float angle = prediction["angle"].get<float>();
        float width = prediction["width"].get<float>();
        float height = prediction["height"].get<float>();
        float x = prediction["x"].get<float>();
        float y = prediction["y"].get<float>();

        prediction_rects.emplace_back(cv::Point2f(x, y), cv::Size2f(width, height), angle);
    }

    return prediction_rects;
}

double CalculatePairIOU(const cv::RotatedRect& first, const cv::RotatedRect& second) {
    std::vector<cv::Point2f> intersection;
    cv::rotatedRectangleIntersection(first, second, intersection);

    if (intersection.empty()) return 0.0;

    double inter_area = cv::contourArea(intersection);
    double union_area = first.size.area() + second.size.area() - inter_area;

    return (union_area > 0) ? (inter_area / union_area) : 0.0;
}

cv::Mat CreateMask(const std::vector<cv::RotatedRect>& ellipses, int width, int height) {
    cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
    for (const auto& elps : ellipses) {
        cv::ellipse(mask, elps, cv::Scalar(255), -1);
    }
    return mask;
}

double CalculateGlobalIoU(std::vector<cv::RotatedRect>& ground_truth,
                          std::vector<cv::RotatedRect>& predictions, int imsize) {
    cv::Mat gt_mask = CreateMask(ground_truth, imsize, imsize);
    cv::Mat pred_mask = CreateMask(predictions, imsize, imsize);

    cv::Mat intersection, union_;
    cv::bitwise_and(gt_mask, pred_mask, intersection);
    cv::bitwise_or(gt_mask, pred_mask, union_);

    double inter_area = cv::countNonZero(intersection);
    double union_area = cv::countNonZero(union_);

    return (union_area > 0) ? (inter_area / union_area) : 0.0;
}

Metrics EvalDetection(std::vector<cv::RotatedRect>& ground_truth,
                      std::vector<cv::RotatedRect>& predictions,
                      int imsize, double iou_threshold = 0.5) {
    Metrics evaluation_result;

    double iou = CalculateGlobalIoU(ground_truth, predictions, imsize);

    int tp = 0, fp = 0, fn = 0;
    std::vector<bool> gt_matched(ground_truth.size(), false);
    std::vector<bool> pred_matched(predictions.size(), false);

    for (size_t i = 0; i < ground_truth.size(); ++i) {
        for (size_t j = 0; j < predictions.size(); ++j) {
            double pair_iou = CalculatePairIOU(ground_truth[i], predictions[j]);
            if (pair_iou >= iou_threshold && !gt_matched[i] && !pred_matched[j]) {
                tp++;
                gt_matched[i] = true;
                pred_matched[j] = true;
            }
        }
        if (!gt_matched[i]) fn++;
    }

    fp = std::count(pred_matched.begin(), pred_matched.end(), false);

    evaluation_result.setValues(iou, tp, fp, fn);
    return evaluation_result;
}

int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    std::string DEFAULT_GT_LST = "/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/test-examples/test-images/ground-truth.lst";
    std::string DEFAULT_PRED_LST = "/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/detections/images/detections.lst";
    std::string DEFAULT_OUTP_PATH = "/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/detections/jsons/result.json";

    std::string gt_lst = DEFAULT_GT_LST;
    std::string pred_lst = DEFAULT_PRED_LST;
    std::string outp_path = DEFAULT_OUTP_PATH;

    if (argc == 4) {
        gt_lst = argv[1];
        pred_lst = argv[2];
        outp_path = argv[3];
    }

    json output_json;

    std::ifstream gt_lst_read(gt_lst);
    std::ifstream pred_lst_read(pred_lst);

    std::string gt_path, pred_path;

    try {
        output_json["samples_metrics"] = json::array();

        int counter = 0;
        double iou_sum = 0.0, precision_sum = 0.0, recall_sum = 0.0, f1_sum = 0.0;

        while (std::getline(gt_lst_read, gt_path) && std::getline(pred_lst_read, pred_path)) {
            std::ifstream ground_truth_read(gt_path);
            json ground_truth_json = json::parse(ground_truth_read);

            int tile_size = ground_truth_json["tile_size"].get<int>();
            int num_tiles = ground_truth_json["tiles"].size();
            int imsize = tile_size * static_cast<int>(std::sqrt(num_tiles));  // Условно, зависит от формата данных

            std::ifstream predictions_read(pred_path);
            json predictions_json = json::parse(predictions_read);

            auto ground_truth_rects = ParseGroundTruth(ground_truth_json);
            auto predicted_rects = ParsePredictions(predictions_json);

            Metrics result = EvalDetection(ground_truth_rects, predicted_rects, imsize);

            json sample_result = {
                {"IoU", result.IoU},
                {"Precision", result.precision},
                {"Recall", result.recall},
                {"F1-score", result.f1}
            };

            output_json["samples_metrics"].push_back(sample_result);

            iou_sum += result.IoU;
            precision_sum += result.precision;
            recall_sum += result.recall;
            f1_sum += result.f1;

            std::cout << "IoU: " << result.IoU << std::endl;
            std::cout << "Precision: " << result.precision << std::endl;
            std::cout << "Recall: " << result.recall << "\n\n";

            counter++;
        }

        if (counter > 0) {
            json avg_metrics = {
                {"IoU", iou_sum / counter},
                {"Precision", precision_sum / counter},
                {"Recall", recall_sum / counter},
                {"F1-score", f1_sum / counter}
            };

            output_json["avg"] = avg_metrics;
        }

        std::ofstream out(outp_path);
        out << output_json.dump(4);
    }
    catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    return 0;
}
