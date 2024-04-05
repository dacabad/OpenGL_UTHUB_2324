#pragma once
#include <string>

class Shader
{
	unsigned int id_;
	std::string sourceFile_;

public:

	Shader(const std::string& sourceFile, unsigned int shaderType);

	unsigned int getShaderId() const { return id_; }

	void compileShader();

	void validateShader();

private:

	void loadShaderFromFile(const std::string& pathToShaderFile, std::string& outShaderContents);
};

class ShaderProgram
{
	unsigned int id_;

public:

	ShaderProgram();

	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

	void attachShader(unsigned int shaderId);
	void attachShader(const Shader& shader);

	void linkProgram();

	void useProgram();

	unsigned int getShaderId() const { return id_; }
};


