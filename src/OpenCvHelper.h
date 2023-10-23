#pragma once
#include <opencv2/core/mat.hpp>

class OpenCvHelper
{
public:
	cv::Mat GetGrayScaled(const std::vector<std::vector<double>>& source) const;
	cv::Mat Crop(const cv::Mat& image, cv::Rect roi);
	bool Match(const cv::Mat& image, const cv::Mat& templ, const double threshold, cv::Point* match, double* max) const;
	void SaveImage(const cv::Mat& image, const std::string& folder, const std::string& file_name) const;
};
