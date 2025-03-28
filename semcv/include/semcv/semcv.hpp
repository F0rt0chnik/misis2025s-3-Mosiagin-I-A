#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace semcv {
    std::string strid_from_mat(const cv::Mat& img, int n = 4);
    std::vector<fs::path> get_list_of_file_paths(const fs::path& path_lst);
}