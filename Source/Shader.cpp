#include "Shader.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

Shader::Shader(const std::string& sourceFile, unsigned int shaderType)
{
	sourceFile_ = sourceFile;

	id_ = glCreateShader(shaderType);

	std::string outShader;
	loadShaderFromFile(sourceFile, outShader);

	const char* shaderSource = outShader.c_str();

	glShaderSource(id_, 1, &shaderSource, nullptr);
	compileShader();
}

void Shader::compileShader()
{
	glCompileShader(id_);
	validateShader();
}

void Shader::validateShader()
{
	GLint status;
	glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cout << "Shader compilation for file:\n " << sourceFile_ << std::endl;
	}
}

void Shader::loadShaderFromFile(const std::string& pathToShaderFile, std::string& outShaderContents)
{
	std::ifstream istream;
	istream.open(pathToShaderFile);

	if(istream.is_open())
	{
		std::stringstream sstream;

		sstream << istream.rdbuf();

		outShaderContents = sstream.str();
	}
	else
	{
		assert(false, "Could not open file");
	}

	istream.close();
}

ShaderProgram::ShaderProgram()
{
	id_ = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : ShaderProgram()
{
	attachShader(vertexShader);
	attachShader(fragmentShader);
	linkProgram();
}

void ShaderProgram::attachShader(unsigned int shaderId)
{
	glAttachShader(id_, shaderId);
}

void ShaderProgram::attachShader(const Shader& shader)
{
	//glAttachShader(id_, shader.getShaderId());
	attachShader(shader.getShaderId());
}

void ShaderProgram::linkProgram()
{
	glLinkProgram(id_);
}

void ShaderProgram::useProgram()
{
	glUseProgram(id_);
}
