#include <iostream>
#include <opencv2/opencv.hpp>
#include <semcv/semcv.hpp>


int main() {
	cv::Mat orig = cv::imread("../test3/greycat.jpg", cv::IMREAD_COLOR);
	cv::Mat contrasted = autocontrast(orig, 0.05, 0.05);

	std::vector<cv::Mat> orig_channels;
	cv::split(orig, orig_channels);

	std::vector<cv::Mat> contr_channels;
	cv::split(contrasted, contr_channels);

	cv::Mat orig_hist_union;
	cv::Mat contr_hist_union;
	
	cv::Mat blue_orig_hist = make_hist(orig_channels[0], cv::Scalar(255, 255, 255), cv::Scalar(255, 0, 0));
	cv::Mat red_orig_hist = make_hist(orig_channels[1], cv::Scalar(255, 255, 255), cv::Scalar(0, 255, 0));
	cv::Mat green_orig_hist = make_hist(orig_channels[2], cv::Scalar(255, 255, 255), cv::Scalar(0, 0, 255));

	cv::hconcat(blue_orig_hist, red_orig_hist, orig_hist_union);
	cv::hconcat(orig_hist_union, green_orig_hist, orig_hist_union);

	cv::Mat blue_contr_hist = make_hist(contr_channels[0], cv::Scalar(255, 255, 255), cv::Scalar(255, 0, 0));
	cv::Mat red_contr_hist = make_hist(contr_channels[1], cv::Scalar(255, 255, 255), cv::Scalar(0, 255, 0));
	cv::Mat green_contr_hist = make_hist(contr_channels[2], cv::Scalar(255, 255, 255), cv::Scalar(0, 0, 255));

	cv::hconcat(blue_contr_hist, red_contr_hist, contr_hist_union);
	cv::hconcat(contr_hist_union, green_contr_hist, contr_hist_union);

	cv::imwrite("../report-imgs/grey/cat-good-contrasted.jpg", contrasted);
	cv::imwrite("../report-imgs/grey/cat-orig-hists.jpg", orig_hist_union);
	cv::imwrite("../report-imgs/grey/cat-good-contr-hists.jpg", contr_hist_union);


	//cv::Mat good_contrasted = autocontrast_rgb(orig, 0.1, 0.1);
	
	cv::imshow("orig", orig);
	cv::imshow("naive_contrasted", contrasted);
	cv::imshow("orig_hists", orig_hist_union);
	cv::imshow("cont_hists", contr_hist_union);
	//cv::imshow("good_contrasted", good_contrasted);

	cv::waitKey(0);
}