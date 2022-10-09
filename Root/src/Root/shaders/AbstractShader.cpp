#include "AbstractShader.h"

AbstractShader::~AbstractShader()
{
	deleteProgram();
}

void AbstractShader::use()
{
	glUseProgram(ID);
}

void AbstractShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void AbstractShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void AbstractShader::setDouble(const std::string& name, double value) const
{
	glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}
void AbstractShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void AbstractShader::setVector2(const std::string& name, float v1, float v2) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
}
void AbstractShader::setVector2(const std::string& name, glm::vec2 v) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v.x, v.y);
}
void AbstractShader::setVector3(const std::string& name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void AbstractShader::setVector3(const std::string& name, glm::vec3 v) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}
void AbstractShader::setVector3(const std::string& name, glm::ivec3 v) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), (float)v.x, (float)v.y, (float)v.z);
}
void AbstractShader::setMat4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void AbstractShader::deleteProgram()
{
	glDeleteProgram(ID);
}

unsigned int AbstractShader::compileShader(GLenum type, const char* code)
{
	unsigned int id;
	int success;
	char infoLog[512];

	id = glCreateShader(type);
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);
	// Printing errors:
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		Logger::logError("shader compilation failed." + std::string(infoLog));
	}
	return id;
}

bool AbstractShader::replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	while (start_pos != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos = str.find(from);
	}
	return true;
}

std::string AbstractShader::readFile(const char* shaderPath)
{
	/* Retrieving the shader data from the file */
	std::string shaderCode;
	std::ifstream shaderFile;

	// Ensuring ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Opening files
		shaderFile.open(shaderPath);

		// Reading file into stream
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		// Closing file handler
		shaderFile.close();

		// Converting the stream into a string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		Logger::logError("Error: shader file not correctly read.");
	}
	return shaderCode;
}

void AbstractShader::linkProgram()
{
	int success;
	char infoLog[512];

	glLinkProgram(ID);

	/* Printing linking errors if any */

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Getting the error log
		glGetProgramInfoLog(ID, 512, NULL, infoLog);

		// Printing the error log
		Logger::logError("Error: shader program linking failed.");
		Logger::logError(infoLog);
		return;
	}
}
