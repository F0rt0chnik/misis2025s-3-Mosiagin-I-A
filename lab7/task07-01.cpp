#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>


using json = nlohmann::json;


std::vector<std::vector<cv::Point>> DetectEllipseContours(const cv::Mat& inputImage) {
    cv::Mat gray, blurred, binary, processed;

    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, blurred, cv::Size(25, 25), 0);
    cv::threshold(blurred, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(17, 17));
    cv::morphologyEx(binary, processed, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel);


    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(processed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> filtered_contours;

    for (const auto& contour : contours) {
        if (cv::contourArea(contour) > 450) {
            filtered_contours.push_back(contour);
        }
    }

    return filtered_contours;
}


void SaveResultsToJson(const std::vector<std::vector<cv::Point>>& contours, const std::string& outputPath) {
    json j;

    for (const auto& contour : contours) {
        json contourJson = json::array();
        for (const auto& pt : contour) {
            contourJson.push_back({ {"x", pt.x}, {"y", pt.y} });
        }
        j.push_back(contourJson);
    }

    std::ofstream outFile(outputPath);

    outFile << j.dump(4);
    outFile.close();
}



int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    try {
        if (argc > 2) {
            cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);

            std::vector<std::vector<cv::Point>> contours = DetectEllipseContours(img);

            SaveResultsToJson(contours, argv[2]);

            cv::drawContours(img, contours, -1, cv::Scalar(0, 255, 0), 2);

            cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab7/outputs/images/lab7_result_4.jpg", img);
            cv::imshow("img", img);
            cv::waitKey(0);
        }
        else {
            cv::Mat img = cv::imread("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab4/test-examples/test-images/4.jpg", cv::IMREAD_COLOR);

            std::vector<std::vector<cv::Point>> contours = DetectEllipseContours(img);

            SaveResultsToJson(contours, "/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab7/outputs/jsons/4.json");

            cv::drawContours(img, contours, -1, cv::Scalar(0, 255, 0), 2);

            cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab7/outputs/images/4.jpg", img);
            cv::imshow("img", img);
            cv::waitKey(0);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
