#include <semcv/semcv.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <random>


std::string strid_from_mat(const cv::Mat& img, const int n) {
	std::string strid = "";

	int w = img.cols;
	int h = img.rows;
	int channels = img.channels();

	std::string w_string = std::to_string(w);
	std::string h_string = std::to_string(h);
	std::string c_string = std::to_string(channels);

	while (w_string.length() != n) {
		w_string = "0" + w_string;
	}

	while (h_string.length() != n) {
		h_string = "0" + h_string;
	}

	std::string type_string;

	switch (img.depth()) {
	case CV_8U:
		type_string = "uint08";
		break;
	case CV_8S:
		type_string = "sint08";
		break;
	case CV_16U:
		type_string = "uint16";
		break;
	case CV_16S:
		type_string = "sint16";
		break;
	case CV_32S:
		type_string = "sint32";
		break;
	case CV_32F:
		type_string = "real32";
		break;
	case CV_64F:
		type_string = "real64";
		break;
	}

	strid = w_string + 'x' + h_string + '.' + c_string + '.' + type_string;

	return strid;
}


std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst) {
	std::vector<std::filesystem::path> file_paths;

	std::ifstream lstfile;

	lstfile.open(path_lst);
	std::string line;

	if (!lstfile.is_open()) {
		std::cout << "Unable to open this file";
	}
	else {
		while (std::getline(lstfile, line)) {
			std::filesystem::path current_file_path = path_lst.parent_path().concat('/' + line);
			file_paths.push_back(current_file_path);
		}
	}
	return file_paths;
}


cv::Mat generate_grey_striped_img() {
	const int rows = 30;
	const int cols = 768;

	cv::Mat img(rows, cols, CV_8U);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = j / 3;
		}
	}

	return img;
}


cv::Mat make_gamma_correction(const cv::Mat& original, double gamma) {
	cv::Mat corrected = cv::Mat(original.size(), original.type());

	for (int i = 0; i < original.rows; i++) {
		for (int j = 0; j < original.cols; j++) {
			double original_value = original.at<uchar>(i, j);
			corrected.at<uchar>(i, j) = 255.0 * pow(original_value / 255, gamma);
		}
	}

	return corrected;
}


cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2) {
	cv::Mat img = cv::Mat(256, 256, CV_8UC1, cv::Scalar(lev0));

	cv::Point upLeftPoint = cv::Point(23, 23);
	cv::Point downRightPoint = cv::Point(233, 233);
	cv::Rect square = cv::Rect(upLeftPoint, downRightPoint);

	cv::rectangle(img, square, cv::Scalar(lev1), -1);

	int circleRadius = 83;
	cv::Point centerPoint = cv::Point(128, 128);

	cv::circle(img, centerPoint, circleRadius, cv::Scalar(lev2), -1);

	return img;
}


void create_tgtimg00_masks(cv::Mat& backgr_mask, cv::Mat& square_mask, cv::Mat& circle_mask) {
	/*
	������� ��� �������� ����� ��������������� �������� gen_tgtimg00 �����������.
	����� ��� �������� ��������� �� ���������� ��������.
	*/
	int circleRadius = 83;
	cv::Point centerPoint = cv::Point(128, 128);
	circle_mask = cv::Mat::zeros(256, 256, CV_8UC1);
	cv::circle(circle_mask, centerPoint, circleRadius, cv::Scalar(255), -1);

	cv::Point upLeftPoint = cv::Point(23, 23);
	cv::Point downRightPoint = cv::Point(233, 233);
	cv::Rect square = cv::Rect(upLeftPoint, downRightPoint);
	square_mask = cv::Mat::zeros(256, 256, CV_8UC1);
	cv::rectangle(square_mask, square, cv::Scalar(255), -1);
	cv::subtract(square_mask, circle_mask, square_mask);

	backgr_mask = cv::Mat::zeros(256, 256, CV_8UC1);
	cv::rectangle(backgr_mask, square, cv::Scalar(255), -1);
	cv::bitwise_not(backgr_mask, backgr_mask);
}


cv::Mat add_noise_gau(const cv::Mat& img, const int std, const int seed) {
	cv::Mat noisy_img = img.clone();

	std::default_random_engine generator;
	generator.seed(seed);
	std::normal_distribution<double> z_dist(0, std);

	for (int i = 0; i < noisy_img.rows; ++i) {
		for (int j = 0; j < noisy_img.cols; ++j) {
			double noise = z_dist(generator);
			double noisy_value = noisy_img.at<uchar>(i, j) + noise;

			if (noisy_value < 0)
				noisy_value = 0;

			else if (noisy_value > 255)
				noisy_value = 255;

			noisy_img.at<uchar>(i, j) = static_cast<uchar>(noisy_value);
		}
	}

	return noisy_img;
}


cv::Mat make_hist(const cv::Mat& src, const cv::Scalar& bg_color, const cv::Scalar& hist_color) {
	int hist_size = 256;
	float range[] = { 0, 256 };
	const float* hist_range = { range };

	cv::Mat hist;
	calcHist(&src, 1, 0, cv::Mat(), hist, 1, &hist_size, &hist_range);

	cv::Mat hist_image(256, 256, CV_8UC3, bg_color);

	double maxVal;
	minMaxLoc(hist, 0, &maxVal);
	hist = hist * (250.0 / maxVal);

	for (int i = 0; i < 256; i++) {
		line(
			hist_image,
			cv::Point(i, 256 - cvRound(hist.at<float>(i))), 
			cv::Point(i, 256),                              
			hist_color
		);
	}

	return hist_image;
}


cv::Mat autocontrast(const cv::Mat& img, double q_black, double q_white) {
	std::vector<cv::Mat> channels;
	cv::split(img, channels);

	for (int c = 0; c < channels.size(); c++) {
		// ���������� �����������
		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		cv::Mat hist;
		cv::calcHist(&channels[c], 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);


		cv::Mat cumulative_hist = hist.clone();

		for (int i = 1; i < histSize; ++i) {
			cumulative_hist.at<float>(i) += cumulative_hist.at<float>(i - 1);
		}

		int total = channels[c].rows * channels[c].cols;
		float black_threshold = q_black * total;
		float white_threshold = (1 - q_white) * total;

		// q_black_value - ��������, ��������������� �������� q_black
		int q_black_value = 0;
		while (q_black_value < histSize && cumulative_hist.at<float>(q_black_value) < black_threshold) {
			q_black_value++;
		}

		// q_white_value - ��������, ��������������� �������� q_white
		int q_white_value = 0;
		while (q_white_value < histSize && cumulative_hist.at<float>(q_white_value) < white_threshold) {
			q_white_value++;
		}

		// ���������� ��������� ����������
		cv::Mat result;
		channels[c].convertTo(result, CV_32F);
		result = (result - q_black_value) * (255.0 / (q_white_value - q_black_value));
		result = cv::max(0, cv::min(255, result));
		result.convertTo(result, CV_8U);
		channels[c] = result;
	}

	cv::Mat contrasted;
	cv::merge(channels, contrasted);

	return contrasted;
}


cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white) {
	// ����������� ������� ��� ���� �������
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	cv::Mat hist;

	int channels[] = { 0, 1, 2 };
	cv::calcHist(&img, 1, channels, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

	cv::Mat cumulative_hist;
	hist.copyTo(cumulative_hist);
	for (int i = 1; i < histSize; ++i) {
		cumulative_hist.at<float>(i) += cumulative_hist.at<float>(i - 1);
	}

	int total_pixels = img.rows * img.cols * 3;
	float black_threshold = q_black * total_pixels;
	float white_threshold = (1 - q_white) * total_pixels;

	// q_black_value - ��������, ��������������� �������� q_black
	int q_black_value = 0;
	while (q_black_value < histSize && cumulative_hist.at<float>(q_black_value) < black_threshold) {
		q_black_value++;
	}

	// q_white_value - ��������, ��������������� �������� q_white
	int q_white_value = 0;
	while (q_white_value < histSize && cumulative_hist.at<float>(q_white_value) < white_threshold) {
		q_white_value++;
	}

	// ���������� ����������� ��� ���� �������
	std::vector<cv::Mat> img_channels;
	cv::split(img, img_channels);

	for (auto& channel : img_channels) {
		channel.convertTo(channel, CV_32F);
		channel = (channel - q_black_value) * (255.0 / (q_white_value - q_black_value));
		channel = cv::max(0, cv::min(255, channel));
		channel.convertTo(channel, CV_8U);
	}

	cv::Mat result;
	cv::merge(img_channels, result);
	return result;
}