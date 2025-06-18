#include "semcv/semcv.hpp"

#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>

cv::Mat draw_histogram_colors(const cv::Mat& image, int height, int width, int hmax, cv::Scalar bg_color) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels); // BGR

    int bins = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    std::vector<cv::Mat> hist(3);
    for (int i = 0; i < 3; ++i) {
        cv::calcHist(&channels[i], 1, 0, cv::Mat(), hist[i], 1, &bins, &histRange);
        cv::normalize(hist[i], hist[i], 0, hmax, cv::NORM_MINMAX);
    }

    cv::Mat canvas(height, width, CV_8UC3, bg_color);
    int bin_w = cvRound((double)width / bins);

    for (int i = 1; i < bins; ++i) {
        int x1 = bin_w * (i - 1);
        int x2 = bin_w * i;

        cv::line(canvas, cv::Point(x1, height - cvRound(hist[2].at<float>(i - 1))),
                 cv::Point(x2, height - cvRound(hist[2].at<float>(i))), cv::Scalar(0, 0, 255), 2);

        cv::line(canvas, cv::Point(x1, height - cvRound(hist[1].at<float>(i - 1))),
                 cv::Point(x2, height - cvRound(hist[1].at<float>(i))), cv::Scalar(0, 255, 0), 2);

        cv::line(canvas, cv::Point(x1, height - cvRound(hist[0].at<float>(i - 1))),
                 cv::Point(x2, height - cvRound(hist[0].at<float>(i))), cv::Scalar(255, 0, 0), 2);
    }

    // Добавляем сетку и подписи
    for (int i = 0; i <= 255; i += 64) {
        int x = cvRound(i * width / 256.0);
        cv::line(canvas, cv::Point(x, height), cv::Point(x, height - 5), cv::Scalar(0, 0, 0), 1);
        cv::putText(canvas, std::to_string(i), cv::Point(x + 2, height - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 0), 1);
    }

    for (int i = 64; i < 256; i += 64) {
        int x = cvRound(i * width / 256.0);
        cv::line(canvas, cv::Point(x, 0), cv::Point(x, height), cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
    }

    return canvas;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage:\n"
                  << "  task08 <input_image> <reference_image> <output_prefix>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string reference_path = argv[2];
    std::string output_prefix = argv[3];

    try {
        std::filesystem::path out_dir = std::filesystem::path(output_prefix).parent_path();
        if (!out_dir.empty() && !std::filesystem::exists(out_dir)) {
            std::filesystem::create_directories(out_dir);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to create output directories: " << e.what() << '\n';
        return 1;
    }

    cv::Mat input = cv::imread(input_path);
    cv::Mat reference = cv::imread(reference_path);

    if (input.empty() || reference.empty()) {
        std::cerr << "Error: Failed to load input or reference image.\n";
        return 1;
    }

    if (input.size() != reference.size()) {
        std::cerr << "Error: Input and reference images must have the same size.\n";
        return 1;
    }

    // === Цветокоррекция ===
    cv::Mat gray_world_corrected = gray_world_correction(input);
    cv::Mat stretched_corrected = histogram_stretching(input, reference);

    // === Сохранение результатов ===
    cv::imwrite(output_prefix + "_gray_world.png", gray_world_corrected);
    cv::imwrite(output_prefix + "_stretched.png", stretched_corrected);

    // === Гистограммы ===
    int hist_h = 512, hist_w = 512, hist_max = 500;
    cv::Scalar bg_color(240, 240, 240);

    cv::imwrite(output_prefix + "_hist_input.png", draw_histogram_colors(input, hist_h, hist_w, hist_max, bg_color));
    cv::imwrite(output_prefix + "_hist_reference.png", draw_histogram_colors(reference, hist_h, hist_w, hist_max, bg_color));
    cv::imwrite(output_prefix + "_hist_gray_world.png", draw_histogram_colors(gray_world_corrected, hist_h, hist_w, hist_max, bg_color));
    cv::imwrite(output_prefix + "_hist_stretched.png", draw_histogram_colors(stretched_corrected, hist_h, hist_w, hist_max, bg_color));

    // === Метрики ===
    img_metrics m1 = compare_images_color_metrics(reference, gray_world_corrected);
    img_metrics m2 = compare_images_color_metrics(reference, stretched_corrected);

    std::cout << "[Gray World Correction]\n"
              << "  MAE  (R,G,B): " << m1.mae_r << ", " << m1.mae_g << ", " << m1.mae_b << "\n"
              << "  PSNR (R,G,B): " << m1.psnr_r << ", " << m1.psnr_g << ", " << m1.psnr_b << "\n\n";

    std::cout << "[Histogram Matching]\n"
              << "  MAE  (R,G,B): " << m2.mae_r << ", " << m2.mae_g << ", " << m2.mae_b << "\n"
              << "  PSNR (R,G,B): " << m2.psnr_r << ", " << m2.psnr_g << ", " << m2.psnr_b << "\n";

    return 0;
}
