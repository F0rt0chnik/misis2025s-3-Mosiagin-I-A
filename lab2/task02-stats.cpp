#include <opencv2/opencv.hpp>
#include <semcv/semcv.hpp>
#include <iostream>

// Псевдонимы для semcv
inline cv::Mat create_target_image(int a, int b, int c) {
    return gen_tgtimg00(a, b, c);
}

inline void generate_target_masks(cv::Mat& a, cv::Mat& b, cv::Mat& c) {
    create_tgtimg00_masks(a, b, c);
}

inline cv::Mat apply_gaussian_noise(const cv::Mat& img, int sigma) {
    return add_noise_gau(img, sigma);
}

struct RegionStats {
    double mean_value;
    double std_dev;
};

RegionStats compute_region_stats(const cv::Mat& image, const cv::Mat& region_mask) {
    cv::Scalar mean, stddev;
    cv::meanStdDev(image, mean, stddev, region_mask);
    return {mean[0], stddev[0]};
}

void analyze_image(int level0, int level1, int level2) {
    std::cout << "\nImage Parameters: "
              << "L0=" << level0 << " "
              << "L1=" << level1 << " "
              << "L2=" << level2 << "\n\n";
    
    cv::Mat image = create_target_image(level0, level1, level2);
    cv::Mat background_mask, square_mask, circle_mask;
    generate_target_masks(background_mask, square_mask, circle_mask);
    
    const std::vector<int> noise_sigmas = {3, 7, 15};
    const std::vector<std::string> region_names = {"Circle", "Inner Square", "Background"};
    const std::vector<cv::Mat> region_masks = {circle_mask, square_mask, background_mask};

    for (int sigma : noise_sigmas) {
        cv::Mat noisy_image = apply_gaussian_noise(image, sigma);
        std::cout << "Noise Level (σ=" << sigma << "):\n";
        
        for (size_t i = 0; i < region_names.size(); ++i) {
            RegionStats stats = compute_region_stats(noisy_image, region_masks[i]);
            std::cout << region_names[i] << ": "
                      << "μ=" << stats.mean_value << " "
                      << "σ=" << stats.std_dev << "\n";
        }
        std::cout << std::endl;
    }
}

int main() {
    const std::vector<std::vector<int>> image_params = {
        {0, 127, 255},
        {20, 127, 235},
        {55, 127, 200},
        {90, 127, 165}
    };

    for (const auto& params : image_params) {
        analyze_image(params[0], params[1], params[2]);
    }
}
