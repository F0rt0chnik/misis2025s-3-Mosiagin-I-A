// apps/task01-02/main.cpp
#include "semcv/semcv.hpp"
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output_path.png>\n";
        return 1;
    }

    cv::Mat stripes = semcv::generate_stripes();
    std::vector<double> gammas = {1.8, 2.0, 2.2, 2.4, 2.6};
    std::vector<cv::Mat> images;
    images.push_back(stripes);

    for (auto gamma : gammas) {
        cv::Mat corrected;
        semcv::gamma_correct(stripes, corrected, gamma);
        images.push_back(corrected);
    }

    cv::Mat collage;
    cv::vconcat(images, collage);
    cv::imwrite(argv[1], collage);
    return 0;
}