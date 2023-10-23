#pragma once
#include <matio.h>
#include <string>
#include <vector>

class MatFileWrapper
{
	mat_t* _mat_file;

public:
	explicit MatFileWrapper(const std::string& file_name);
	std::vector<std::vector<double>> GetVariableAsDouble2d(const std::string& var_name) const;
	~MatFileWrapper();
};
