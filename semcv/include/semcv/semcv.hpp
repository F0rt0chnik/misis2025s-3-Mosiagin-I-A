#pragma once
#ifndef SEMCV_LIB_2373
#define SEMCV_LIB_2373

#include <fstream>
#include <opencv2/opencv.hpp>
#include <filesystem>


std::string strid_from_mat(const cv::Mat& img, const int n = 4);


std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst);


cv::Mat generate_grey_striped_img();


cv::Mat make_gamma_correction(const cv::Mat& original, double gamma);


cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2);

void create_tgtimg00_masks(cv::Mat& backgr_mask, cv::Mat& square_mask, cv::Mat& circle_mask);


cv::Mat add_noise_gau(const cv::Mat& img, const int std, const int seed = 42);


cv::Mat make_hist(const cv::Mat& src, const cv::Scalar& bg_color = cv::Scalar(255), const cv::Scalar& hist_color = cv::Scalar(0));


cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white);


cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white);

#endif