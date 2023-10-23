#include "MatFileWrapper.h"

MatFileWrapper::MatFileWrapper(const std::string& file_name)
{
	const auto mat = Mat_Open(file_name.c_str(), MAT_ACC_RDONLY);
	if (mat == nullptr) 
	{
		throw std::exception("Error opening mat file");
	}
	_mat_file = mat;
}

std::vector<std::vector<double>> MatFileWrapper::GetVariableAsDouble2d(const std::string& var_name) const
{
	const matvar_t* var = Mat_VarRead(_mat_file, var_name.c_str());
	if (var == nullptr)
	{
		throw std::exception("Error reading variable");
	}

	const auto elements = static_cast<double*>(var->data);
	const auto rows = var->dims[0];
	const auto columns = var->nbytes / var->data_size / rows;

	std::vector<std::vector<double>> image_source(rows);

	for (size_t i = 0; i < rows; ++i)
	{
		image_source[i] = std::vector<double>(columns);
	}

	for (size_t j = 0; j < columns; ++j)
	{
		for (size_t i = 0; i < rows; ++i)
		{
			image_source[i][j] = elements[j * rows + i];
		}
	}

	return image_source;
}

MatFileWrapper::~MatFileWrapper()
{
	Mat_Close(_mat_file);
}
