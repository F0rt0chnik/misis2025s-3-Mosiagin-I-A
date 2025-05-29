#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>


using json = nlohmann::json;


std::vector<cv::RotatedRect> DetectEllipses(const cv::Mat& inputImage) {
    cv::Mat gray, blurred, binary, processed;

    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, blurred, cv::Size(25, 25), 0);
    cv::threshold(blurred, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);


    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(17, 17));
    cv::morphologyEx(binary, processed, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel);


    cv::Mat labels, stats, centroids;
    int numComponents = connectedComponentsWithStats(processed, labels, stats, centroids);

    std::vector<cv::RotatedRect> result;


    int minArea = 900;

    for (int i = 1; i < numComponents; ++i) { 
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area < minArea) continue;


        cv::Mat componentMask = (labels == i);


        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(componentMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (!contours.empty() && contours[0].size() >= 5) {
            cv::RotatedRect ellipse = cv::fitEllipse(contours[0]);
            result.push_back(ellipse);
        }
    }

    return result;
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
    try {
        if (argc > 2) {
            cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

            std::vector<cv::RotatedRect> predicted_rects = DetectEllipses(img);

            SaveResultsToJson(predicted_rects, argv[2]);

            for (const auto& rect : predicted_rects) {
                cv::ellipse(img, rect, cv::Scalar(0, 0, 255));
                cv::circle(img, rect.center, 5, cv::Scalar(0, 0, 255), -1);
            }

            cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/detections/images/detection_result4.jpg", img);
            cv::imshow("img", img);
            cv::waitKey(0);
        }
        else {
            cv::Mat img = cv::imread("./test-examples/test-images/4.jpg", cv::IMREAD_COLOR);

            std::vector<cv::RotatedRect> predicted_rects = DetectEllipses(img);

            //saveResultsToJson(predicted_rects, argv[2]);

            for (const auto& rect : predicted_rects) {
                cv::ellipse(img, rect, cv::Scalar(0, 0, 255));
                cv::circle(img, rect.center, 5, cv::Scalar(0, 0, 255), -1);
            }

            cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/detections/images/detection_result4.jpg", img);
            cv::imshow("img", img);
            cv::waitKey(0);
            //std::cout << "Not enough parameters" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}