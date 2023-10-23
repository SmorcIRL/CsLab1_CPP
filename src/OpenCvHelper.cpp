#include "OpenCvHelper.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>

cv::Mat OpenCvHelper::GetGrayScaled(const std::vector<std::vector<double>>& source) const
{
	const auto rows_count = source.size();
	const auto columns_count = source[0].size();

	cv::Mat matrix(rows_count, columns_count, CV_32FC1);

	for (size_t i = 0; i < rows_count; ++i)
	{
		for (size_t j = 0; j < columns_count; ++j)
		{
			matrix.at<float>(i, j) = static_cast<float>(source[i][j]);
		}
	}

	double min;
	double max;
	minMaxIdx(matrix, &min, &max);
	cv::Mat result;

	matrix.convertTo(result, CV_8UC1, 255 / max);

	return result;
}

cv::Mat OpenCvHelper::Crop(const cv::Mat& image, const cv::Rect roi)
{
	const cv::Mat src_roi(image, roi);
	cv::Mat result(roi.size(), CV_8UC1);

	src_roi.copyTo(result);

	return result;
}

bool OpenCvHelper::Match(const cv::Mat& image, const cv::Mat& templ, const double threshold, cv::Point* match, double* max) const
{
	cv::Mat result(image.size(), CV_32FC1);

	matchTemplate(image, templ, result, cv::TM_CCOEFF_NORMED);

	double min;
	cv::Point min_loc;
	minMaxLoc(result, &min, max, &min_loc, match);

	return *max >= threshold;
}

void OpenCvHelper::SaveImage(const cv::Mat& image, const std::string& folder, const std::string& file_name) const
{
	const std::filesystem::path dir(folder);
	const std::filesystem::path file(file_name);
	imwrite((dir / file).string(), image);
}
