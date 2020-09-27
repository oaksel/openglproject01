#pragma once
#include "prefix.h"

struct shaderCode
{
	std::string vertexPart;
	std::string fragmentPart;
};

class ShaderProgram
{
private:
	uint shaderProgramID;

	uint vertexShaderID;
	uint fragmentShaderID;

	shaderCode sourceCode;
	std::string filePath;
public:
	ShaderProgram(std::string sourceCodeFilePath);
	~ShaderProgram();

	void use();
	void disUse();

	int getUniformLocation(const std::string uniformName);
	void setUniform1i(int uniformLocation, int value);
	void setUniform1f(int uniformLocation, float value);
	void setUniformMat4f(int uniformLocation, glm::mat4 matrix);
	void setUniform4f(int uniformLocation, glm::vec4 values);
	void setUniform3f(int uniformLocation, glm::vec3 values);
	// TODO more uniform functions

	void setMaterial(std::vector<int> uniformLocation, material material);
	void setLight(std::vector<int> uniformLocation, light light);
private:
	uint CreateShader(GLenum type, std::string content);
	bool CompileShader(uint shaderID);
	bool LinkShaderProgram();
	shaderCode ParseShaderSourceCode();

};

