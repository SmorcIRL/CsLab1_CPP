#include "AppConfigurationFactory.h"
#include "AppConfiguration.h"
#include <sstream>

AppConfiguration AppConfigurationFactory::GetConfiguration(const int argc, char* argv[])
{
	if (argc < 7)
	{
		throw std::exception("Missing arguments");
	}

	AppConfiguration configuration;

	configuration.MatFileLocation = argv[1];
	configuration.Image1VariableName = argv[2];
	configuration.Image2VariableName = argv[3];
	configuration.ImageSaveLocation = argv[4];
	configuration.Threshold = std::stod(argv[5]);
	configuration.TemplateArea = ParseRectString(argv[6]);

	if (configuration.Threshold < 0 || configuration.Threshold > 1)
	{
		throw std::exception("Threshold must be within [0,1]");
	}

	if (configuration.TemplateArea.x < 0 || 
		configuration.TemplateArea.y < 0 ||
		configuration.TemplateArea.width < 0 || 
		configuration.TemplateArea.height < 0)
	{
		throw std::exception("Invalid template area properties");
	}

	return configuration;
}


cv::Rect AppConfigurationFactory::ParseRectString(const char* arg)
{
	std::stringstream stream(arg);
	std::string word;
	std::vector<int> values;
	while (stream >> word)
	{
		values.push_back(std::stoi(word));
	}

	return
	{
		values[0],
		values[1],
		values[2],
		values[3]
	};
}