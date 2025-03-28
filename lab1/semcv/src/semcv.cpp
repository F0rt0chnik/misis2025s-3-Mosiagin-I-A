#include "semcv/semcv.hpp"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace fs = std::filesystem;

std::string semcv::strid_from_mat(const cv::Mat& img, int n) {
    std::ostringstream oss;
    oss << std::setw(n) << std::setfill('0') << img.cols << "x"
        << std::setw(n) << std::setfill('0') << img.rows << "."
        << img.channels() << ".";

    switch(img.depth()) {
        case CV_8U:  oss << "uint08"; break;
        case CV_8S:  oss << "sint08"; break;
        case CV_16U: oss << "uint16"; break;
        case CV_16S: oss << "sint16"; break;
        case CV_32S: oss << "sint32"; break;
        case CV_32F: oss << "real32"; break;
        case CV_64F: oss << "real64"; break;
        default:     oss << "unknown";
    }

    return oss.str();
}

std::vector<fs::path> semcv::get_list_of_file_paths(const fs::path& path_lst) {
    std::vector<fs::path> paths;
    std::ifstream file(path_lst);
    std::string line;
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path_lst.string());
    }

    while (std::getline(file, line)) {
        if (!line.empty()) {
            paths.push_back(path_lst.parent_path() / line);
        }
    }
    
    return paths;
}