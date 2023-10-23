#include "AppConfigurationFactory.h"
#include "MatFileWrapper.h"
#include "OpenCvHelper.h"
#include <iostream>
#include <format>

int main(int argc, char* argv[])
{
	auto
	[
		MatFileLocation,
		Image1VariableName,
		Image2VariableName,
		ImageSaveLocation,
		Threshold,
		TemplateArea
	] = AppConfigurationFactory::GetConfiguration(argc, argv);

	MatFileWrapper mat_wrapper(MatFileLocation);
	OpenCvHelper open_cv_helper;

	const auto image1_source = mat_wrapper.GetVariableAsDouble2d(Image1VariableName);
	const auto image2_source = mat_wrapper.GetVariableAsDouble2d(Image2VariableName);

	const auto image1_grey_scaled = open_cv_helper.GetGrayScaled(image1_source);
	const auto image2_grey_scaled = open_cv_helper.GetGrayScaled(image2_source);

	if (image1_grey_scaled.size() != image2_grey_scaled.size())
	{
		throw std::exception("Source and target images must have the same size");
	}

	const auto template_image = open_cv_helper.Crop(image1_grey_scaled, TemplateArea);

	cv::Point best_match;
	double best_match_fitness;
	if (!open_cv_helper.Match(image2_grey_scaled, template_image, Threshold, &best_match, &best_match_fitness))
	{
		throw std::exception("Match not found with given parameters. Try lower threshold value");
	}

	const auto best_match_image = open_cv_helper.Crop(image2_grey_scaled,
	{
		best_match.x,
		best_match.y,
		TemplateArea.width,
		TemplateArea.height
	});

	open_cv_helper.SaveImage(image1_grey_scaled, ImageSaveLocation, "image1_grey_scaled.bmp");
	open_cv_helper.SaveImage(image2_grey_scaled, ImageSaveLocation, "image2_grey_scaled.bmp");
	open_cv_helper.SaveImage(template_image, ImageSaveLocation, "template_image.bmp");
	open_cv_helper.SaveImage(best_match_image, ImageSaveLocation, "best_match_image.bmp");

	std::cout << std::format("Best match fitness: {}", best_match_fitness) << std::endl;
	std::cout << std::format("Template location: x={}, y={}", TemplateArea.x, TemplateArea.y) << std::endl;
	std::cout << std::format("Best match location: x={}, y={}", best_match.x, best_match.y) << std::endl;
	std::cout << std::format("Shift: x={}, y={}", TemplateArea.x - best_match.x, TemplateArea.y - best_match.y) << std::endl;

	system("pause");
}