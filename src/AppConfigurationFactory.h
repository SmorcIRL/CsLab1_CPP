#pragma once
#include "AppConfiguration.h"

class AppConfigurationFactory
{
	static cv::Rect ParseRectString(const char* arg);
public:
	static AppConfiguration GetConfiguration(int argc, char** argv);
};
