#include "AppConfigurationFactory.h"
#include "MatFileWrapper.h"
#include "OpenCvHelper.h"
#include <iostream>
#include <format>
#include <opencv2/core/utils/logger.hpp>

void GetImagesData(const AppConfiguration& configuration, std::vector<std::vector<double>>& image1_source, std::vector<std::vector<double>>& image2_source)
{
	const MatFileWrapper mat_wrapper(configuration.MatFileLocation);

	image1_source = mat_wrapper.GetVariableAsDouble2d(configuration.Image1VariableName);
	image2_source = mat_wrapper.GetVariableAsDouble2d(configuration.Image2VariableName);
}

int main(int argc, char* argv[])
{
	auto config = AppConfigurationFactory::GetConfiguration(argc, argv);

	std::vector<std::vector<double>> image1_source;
	std::vector<std::vector<double>> image2_source;

	GetImagesData(config, image1_source, image2_source);

	setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
	OpenCvHelper open_cv_helper;

	const auto image1 = open_cv_helper.GetGreyScaled(image1_source);
	const auto image2 = open_cv_helper.GetGreyScaled(image2_source);

	if (image1.size() != image2.size())
	{
		throw std::exception("Source and target images must have the same size");
	}

	const auto template_image = open_cv_helper.Crop(image1, config.TemplateArea);

	cv::Point best_match_location;
	double best_match_fitness;
	if (!open_cv_helper.Match(image2, template_image, config.Threshold, &best_match_location, &best_match_fitness))
	{
		throw std::exception("Match not found with given parameters. Try lower threshold value");
	}

	const auto best_match = open_cv_helper.Crop(image2,
	{
		best_match_location.x,
		best_match_location.y,
		config.TemplateArea.width,
		config.TemplateArea.height
	});

	cv::Point shift(best_match_location.x - config.TemplateArea.x, best_match_location.y - config.TemplateArea.y);

	auto image2_shifted = open_cv_helper.Shift(image2, -shift);
	auto compensation = open_cv_helper.Sum(image1, image2_shifted);

	open_cv_helper.SaveImage(image1, config.ImageSaveLocation, "image1.bmp");
	open_cv_helper.SaveImage(image2, config.ImageSaveLocation, "image2.bmp");
	open_cv_helper.SaveImage(image2_shifted, config.ImageSaveLocation, "image2_shifted.bmp");
	open_cv_helper.SaveImage(compensation, config.ImageSaveLocation, "compensation.bmp");
	open_cv_helper.SaveImage(template_image, config.ImageSaveLocation, "template.bmp");
	open_cv_helper.SaveImage(best_match, config.ImageSaveLocation, "best_match.bmp");

	std::cout << std::format("Best match fitness: {}", best_match_fitness) << std::endl;
	std::cout << std::format("Template location: x={}, y={}", config.TemplateArea.x, config.TemplateArea.y) << std::endl;
	std::cout << std::format("Best match location: x={}, y={}", best_match_location.x, best_match_location.y) << std::endl;
	std::cout << std::format("Shift: x={}, y={}", shift.x, shift.y) << std::endl;

	system("pause");
}
