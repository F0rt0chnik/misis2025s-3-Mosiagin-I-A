#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

using json = nlohmann::json;

// ======== ФУНКЦИИ ДЛЯ ПАРСИНГА ========

std::vector<std::vector<cv::Point>> ParseGroundTruth(const json& data) {
    int tile_size = data["tile_size"];
    std::vector<cv::RotatedRect> rects;

    for (const auto& tile : data["tiles"]) {
        const auto& elps = tile["elps_parameters"];
        const auto& coords = tile["pic_coordinates"];

        int abs_x = coords["col"].get<int>() * tile_size;
        int abs_y = coords["row"].get<int>() * tile_size;
        int elps_x = elps["elps_x"].get<int>();
        int elps_y = elps["elps_y"].get<int>();
        float width = elps["elps_width"].get<float>() * 2;
        float height = elps["elps_height"].get<float>() * 2;
        float angle = elps["elps_angle"].get<float>();

        cv::Point center(elps_x + abs_x, elps_y + abs_y);
        rects.emplace_back(center, cv::Size2f(width, height), angle);
    }

    std::vector<std::vector<cv::Point>> contours;
    for (const auto& rect : rects) {
        std::vector<cv::Point> contour;
        cv::ellipse2Poly(rect.center, cv::Size(rect.size.width / 2, rect.size.height / 2),
                         static_cast<int>(rect.angle), 0, 360, 5, contour);
        if (contour.size() >= 3)
            contours.push_back(contour);
    }
    return contours;
}

std::vector<std::vector<cv::Point>> ParsePredictions(const json& data) {
    std::vector<std::vector<cv::Point>> contours;
    for (const auto& contourJson : data) {
        std::vector<cv::Point> contour;
        for (const auto& pt : contourJson) {
            int x = pt["x"].get<int>();
            int y = pt["y"].get<int>();
            contour.emplace_back(x, y);
        }
        if (contour.size() >= 3)
            contours.push_back(contour);
    }
    return contours;
}

// ======== МЕТРИКИ ========

double ComputeIoU(const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
    std::vector<cv::Point> intersection;
    cv::intersectConvexConvex(a, b, intersection);

    if (intersection.empty()) return 0.0;

    double interArea = cv::contourArea(intersection);
    double unionArea = cv::contourArea(a) + cv::contourArea(b) - interArea;

    if (unionArea <= 0.0) return 0.0;
    return interArea / unionArea;
}

void Evaluate(const std::vector<std::vector<cv::Point>>& gt,
              const std::vector<std::vector<cv::Point>>& preds,
              double iouThreshold,
              json& result) {
    int TP = 0, FP = 0, FN = 0;
    std::vector<bool> matched(gt.size(), false);

    for (const auto& pred : preds) {
        bool found = false;
        for (size_t i = 0; i < gt.size(); ++i) {
            if (!matched[i] && ComputeIoU(pred, gt[i]) >= iouThreshold) {
                matched[i] = true;
                found = true;
                break;
            }
        }
        if (found) TP++;
        else FP++;
    }

    for (bool wasMatched : matched) {
        if (!wasMatched) FN++;
    }

    double precision = TP + FP > 0 ? static_cast<double>(TP) / (TP + FP) : 0.0;
    double recall = TP + FN > 0 ? static_cast<double>(TP) / (TP + FN) : 0.0;
    double f1 = (precision + recall) > 0 ? 2 * precision * recall / (precision + recall) : 0.0;

    result["TruePositives"] = TP;
    result["FalsePositives"] = FP;
    result["FalseNegatives"] = FN;
    result["Precision"] = precision;
    result["Recall"] = recall;
    result["F1-score"] = f1;
}

// ======== MAIN ========

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: ./task07-02 ground_truth.json predictions.json output_metrics.json" << std::endl;
        return 1;
    }

    std::ifstream gtFile(argv[1]);
    std::ifstream predFile(argv[2]);
    if (!gtFile || !predFile) {
        std::cerr << "Error reading input files." << std::endl;
        return 1;
    }

    json gtJson, predJson;
    gtFile >> gtJson;
    predFile >> predJson;

    auto gtContours = ParseGroundTruth(gtJson);
    auto predContours = ParsePredictions(predJson);

    json metrics;
    Evaluate(gtContours, predContours, 0.5, metrics);

    std::ofstream out(argv[3]);
    out << metrics.dump(4);
    out.close();

    std::cout << "Evaluation complete. Metrics saved to: " << argv[3] << std::endl;
    return 0;
}
