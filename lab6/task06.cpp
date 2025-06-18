#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>


using json = nlohmann::json;


std::vector<cv::Mat> CreatePyramid(const cv::Mat& img, int levels, double sigma, double k) {
	std::vector<cv::Mat> pyramid;
	cv::Mat current_level = img.clone();

	for (int i = 0; i < levels; i++) {
		cv::Mat blurred;
		cv::GaussianBlur(current_level, blurred, cv::Size(0, 0), sigma);

		pyramid.push_back(blurred);

		sigma *= k;
	}

	return pyramid;
}


std::vector<cv::Mat> CreateDoGPyramid(const std::vector<cv::Mat>& pyramid) {
	std::vector<cv::Mat> dog_pyramid;
	for (size_t i = 1; i < pyramid.size(); ++i) {
		cv::Mat dog;
		cv::subtract(pyramid[i], pyramid[i - 1], dog);
        dog_pyramid.push_back(dog);
	}

	return dog_pyramid;
}


std::vector<cv::KeyPoint> DetectLocalExtrema(const std::vector<cv::Mat>& dog_pyramid, double threshold, double sigma0, double k) {
    std::vector<cv::KeyPoint> keypoints;

    for (size_t i = 1; i < dog_pyramid.size() - 1; i++) {
        const cv::Mat& prev = dog_pyramid[i - 1];
        const cv::Mat& curr = dog_pyramid[i];
        const cv::Mat& next = dog_pyramid[i + 1];

        for (int y = 1; y < curr.rows - 1; ++y) {
            for (int x = 1; x < curr.cols - 1; ++x) {
                float val = curr.at<float>(y, x);

                // Пропуск слабых откликов
                if (std::abs(val) < threshold)
                    continue;

                bool isExtremum = true;

                // Сравниваем со всеми соседями (3x3x3 куб)
                for (int dy = -1; dy <= 1 && isExtremum; ++dy) {
                    for (int dx = -1; dx <= 1 && isExtremum; ++dx) {
                        for (int dz = -1; dz <= 1 && isExtremum; ++dz) {
                            if (dz == 0 && dx == 0 && dy == 0) continue;

                            float neighborVal = (dz == -1 ? prev : (dz == 1 ? next : curr)).at<float>(y + dy, x + dx);
                            if ((val > 0 && val <= neighborVal) || (val < 0 && val >= neighborVal))
                                isExtremum = false;
                        }
                    }
                }

                if (isExtremum) {
                    // Размер пятна пропорционален σ
                    float sigma = sigma0 * std::pow(k, static_cast<float>(i));
                    float radius = std::sqrt(2.0f) * sigma;
                    keypoints.emplace_back(cv::KeyPoint(cv::Point2f(x, y), 2.0f * radius));
                }
            }
        }
    }

    return keypoints;
}


std::vector<cv::KeyPoint> DetectEllipses(const cv::Mat& input_img,
    int levels = 10,
    double sigma = 1.6,
    double k = std::sqrt(2.0),
    double threshold = 0.03) {

    cv::Mat gray;
    cv::cvtColor(input_img, gray, cv::COLOR_BGR2GRAY);

    cv::Mat floatImg;
    gray.convertTo(floatImg, CV_32F, 1.0 / 255.0);

    auto pyramid = CreatePyramid(floatImg, levels, sigma, k);

    auto dog_pyramid = CreateDoGPyramid(pyramid);

    auto keypoints = DetectLocalExtrema(dog_pyramid, threshold, sigma, k);

    return keypoints;
}


cv::Mat DrawBlobs(const cv::Mat& image, const std::vector<cv::KeyPoint>& keypoints) {
    std::cout << "Draw blobs start" << std::endl;

    cv::Mat out = image.clone();
    for (const auto& kp : keypoints) {
        cv::circle(out, kp.pt, static_cast<int>(kp.size / 2), cv::Scalar(0, 255, 0), 3);
    }
    return out;
}


std::vector<cv::RotatedRect> KeypointsToRR(const std::vector<cv::KeyPoint>& keypoints) {
    std::vector<cv::RotatedRect> ellipses;

    for (const auto& kp : keypoints) {
        float radius = kp.size / 2.0;

        cv::RotatedRect ellipse(kp.pt, cv::Size2f(radius * 2, radius * 2), 0.0f);

        if (ellipse.size.area() >= 900) {
            ellipses.push_back(ellipse);
        }
    }

    return ellipses;
}


void SaveResultsToJson(const std::vector<cv::RotatedRect>& rects, const std::string& outputPath) {
    json outp;
    outp["detections"] = json::array();

    for (const auto& rect : rects) {
        json detection = {
            {"angle", rect.angle},
            {"x", rect.center.x},
            {"y", rect.center.y},
            {"width", rect.size.width},
            {"height", rect.size.height},
            {"area", rect.size.area()}
        };
        outp["detections"].push_back(detection);
    }

    std::ofstream out(outputPath);
    out << outp.dump(4);
}


int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    cv::Mat img = cv::imread(R"(/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/test-examples/test-images/4.jpg)", cv::IMREAD_COLOR);

    auto keypoints = DetectEllipses(img, 16, 1.6, std::sqrt(2), 0.1);

    auto rr = KeypointsToRR(keypoints);

    for (const auto& rect : rr) {
        cv::ellipse(img, rect, cv::Scalar(0, 0, 255));
        cv::circle(img, rect.center, 5, cv::Scalar(0, 0, 255), -1);
    }

    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab6/report-imgs/detection_result_4.jpg", img);
    cv::imshow("img", img);
    cv::waitKey(0);
}