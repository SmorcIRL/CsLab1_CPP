#pragma once
#include <string>
#include <opencv2/core/types.hpp>

struct AppConfiguration
{
    std::string MatFileLocation;
    std::string Image1VariableName;
    std::string Image2VariableName;
    std::string ImageSaveLocation;
    double Threshold;
    cv::Rect TemplateArea;
};
