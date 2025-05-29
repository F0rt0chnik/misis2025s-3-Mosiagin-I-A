#include <opencv2/opencv.hpp>
#include <filesystem>  

cv::Mat generate_tile(const int lev0, const int lev1) {
    cv::Mat img = cv::Mat(127, 127, CV_8UC1, cv::Scalar(lev0));
    int circleRadius = 40;
    cv::Point centerPoint = cv::Point(64, 64);
    cv::circle(img, centerPoint, circleRadius, cv::Scalar(lev1), -1);
    return img;
}

cv::Mat generate_collage() {
    cv::Mat line1, line2, collage;

    cv::Mat tile1 = generate_tile(0, 127);
    cv::Mat tile2 = generate_tile(127, 255);
    cv::Mat tile3 = generate_tile(255, 0);
    cv::hconcat(tile1, tile2, line1);
    cv::hconcat(line1, tile3, line1);

    tile1 = generate_tile(127, 0);
    tile2 = generate_tile(255, 127);
    tile3 = generate_tile(0, 255);
    cv::hconcat(tile1, tile2, line2);
    cv::hconcat(line2, tile3, line2);

    cv::vconcat(line1, line2, collage);
    return collage;
}

int main() {
    cv::Mat collage = generate_collage();
    cv::imshow("Original", collage);

    // Создание папки для сохранения результатов
    std::filesystem::create_directories("output");

    int kernelSize = 2;
    cv::Mat kernel1 = (cv::Mat_<float>(2, 2) << 1.0, 0.0, 0.0, -1.0);
    cv::Mat kernel2 = (cv::Mat_<float>(2, 2) << 0.0, 1.0, -1.0, 0.0);

    cv::Mat filtered1, filtered2, filtered3;

    cv::filter2D(collage, filtered1, -1, kernel1);
    cv::imshow("Filter 1", filtered1);

    cv::filter2D(collage, filtered2, -1, kernel2);
    cv::imshow("Filter 2", filtered2);

    filtered1.convertTo(filtered1, CV_32F);
    filtered2.convertTo(filtered2, CV_32F);
    cv::sqrt(filtered1.mul(filtered1) + filtered2.mul(filtered2), filtered3);
    cv::imshow("Filter 3", filtered3);

    // Цветной результат
    std::vector<cv::Mat> rgb_channels(3, collage.clone());
    rgb_channels[0].convertTo(rgb_channels[0], CV_32F);
    rgb_channels[1].convertTo(rgb_channels[1], CV_32F);
    cv::filter2D(rgb_channels[0], rgb_channels[0], -1, kernel1);
    cv::filter2D(rgb_channels[1], rgb_channels[1], -1, kernel2);
    cv::sqrt(rgb_channels[0].mul(rgb_channels[0]) + rgb_channels[1].mul(rgb_channels[1]), rgb_channels[2]);

    cv::Mat filtered_rgb;
    cv::merge(rgb_channels, filtered_rgb);
    cv::imshow("RGB", filtered_rgb);

    // Сохраняем изображения
    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab05/output/original.png", collage);

    cv::Mat filtered1_8u, filtered2_8u, filtered3_8u, filtered_rgb_8u;
    filtered1.convertTo(filtered1_8u, CV_8U);
    filtered2.convertTo(filtered2_8u, CV_8U);
    filtered3.convertTo(filtered3_8u, CV_8U);
    filtered_rgb.convertTo(filtered_rgb_8u, CV_8U);

    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab05/output/filter1.png", filtered1_8u);
    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab05/output/filter2.png", filtered2_8u);
    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab05/output/filter3.png", filtered3_8u);
    cv::imwrite("/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/lab05/output/filter_rgb.png", filtered_rgb_8u);

    cv::waitKey(0);
    return 0;
}
