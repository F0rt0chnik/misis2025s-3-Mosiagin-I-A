// apps/task01-01/main.cpp
#include "semcv/semcv.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_lst>\n";
        return 1;
    }

    auto paths = semcv::get_list_of_file_paths(argv[1]);
    for (const auto& path : paths) {
        cv::Mat img = cv::imread(path.string(), cv::IMREAD_UNCHANGED);
        if (img.empty()) {
            std::cout << path.filename() << "\tbad (cannot open)\n";
            continue;
        }

        std::string expected_strid = semcv::strid_from_mat(img);
        std::string filename = path.stem().string(); // Предполагается, что имя файла содержит strid

        if (filename.find(expected_strid) != std::string::npos) {
            std::cout << path.filename() << "\tgood\n";
        } else {
            std::cout << path.filename() << "\tbad, should be " << expected_strid << "\n";
        }
    }
    return 0;
}