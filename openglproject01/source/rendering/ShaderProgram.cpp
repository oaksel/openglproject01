#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string _filePath)
{
	filePath = _filePath;
	sourceCode = ParseShaderSourceCode();

	// Get a program object.
	shaderProgramID = glCreateProgram();

	// Create shaders.
	vertexShaderID = CreateShader(GL_VERTEX_SHADER, sourceCode.vertexPart);
	fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, sourceCode.fragmentPart);
	bool shadersCompiled = CompileShader(vertexShaderID) && CompileShader(fragmentShaderID);

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	bool shaderProgramLinked = LinkShaderProgram();

}

ShaderProgram::~ShaderProgram()
{
	disUse();
	//glDeleteProgram(shaderProgramID);
}

void ShaderProgram::use()
{
	glUseProgram(shaderProgramID);
}

void ShaderProgram::disUse()
{
	glUseProgram(GL_NONE);
}

int ShaderProgram::getUniformLocation(const std::string _uniformName)
{
	int location = glGetUniformLocation(shaderProgramID, _uniformName.c_str());

	if (location == -1)
		printf("ERROR::UNIFORM\n%s doesn't exist.\n", _uniformName.c_str());
	return location;
}

void ShaderProgram::setUniform1i(int _uniformLocation, int _value)
{
	use();
	glUniform1i(_uniformLocation, _value);
	disUse();
}

void ShaderProgram::setUniform1f(int _uniformLocation, float _value)
{
	use();
	glUniform1f(_uniformLocation, _value);
	disUse();
}

void ShaderProgram::setUniformMat4f(int _uniformLocation, glm::mat4 _matrix)
{
	use();
	glUniformMatrix4fv(_uniformLocation, 1, GL_FALSE, &_matrix[0][0]);
	disUse();
}

void ShaderProgram::setUniform4f(int _uniformLocation, glm::vec4 _values)
{
	use();
	glUniform4fv(_uniformLocation, 1, &_values[0]);
	disUse();
}

void ShaderProgram::setUniform3f(int _uniformLocation, glm::vec3 _values)
{
	use();
	glUniform3fv(_uniformLocation, 1, &_values[0]);
	disUse();
}

void ShaderProgram::setMaterial(std::vector<int> _uniformLocation, material _material)
{
	use();
	setUniform1f(_uniformLocation[0], _material.ambient);
	setUniform1f(_uniformLocation[1], _material.diffuse);
	setUniform1f(_uniformLocation[2], _material.specular);
	setUniform1f(_uniformLocation[3], _material.shininess);
	disUse();
}

void ShaderProgram::setLight(std::vector<int> _uniformLocation, light _light)
{
	use();
	setUniform3f(_uniformLocation[0], _light.position);
	setUniform3f(_uniformLocation[1], _light.color);
	setUniform1f(_uniformLocation[2], _light.linearAttenuation);
	setUniform1f(_uniformLocation[3], _light.quadraticAttenuation);
	disUse();
}


uint ShaderProgram::CreateShader(GLenum _type, std::string _content)
{
	// Create an empty vertex shader handle
	uint shaderID = glCreateShader(_type);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const char *source = (const char *)_content.c_str();
	glShaderSource(shaderID, 1, &source, 0);

	return shaderID;
}

bool ShaderProgram::CompileShader(uint _shaderID)
{
	glCompileShader(_shaderID);

	int isCompiled;
	glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(_shaderID, maxLength, &maxLength, &infoLog[0]);

		// Don't need the shader anymore.
		glDeleteShader(_shaderID);

		// Use the infoLog as you see fit.
		printf("ERROR::SHADER_SOURCE_CODE\n%s\n", infoLog.data());

		return false;
	}

	return true;
}

bool ShaderProgram::LinkShaderProgram()
{
	// Attach shaders to program
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);

	// Link program
	glLinkProgram(shaderProgramID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	int isLinked;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		int maxLength = 0;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);

		// Don't need the program anymore.
		glDeleteProgram(shaderProgramID);
		// Don't leak shaders either.
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		// Use the infoLog.
		printf("ERROR::SHADER_PROGRAM_LINKING\n%s\n", infoLog.data());

		return false;
	}

	// Always detach shaders after a successful link.
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	return true;
}

shaderCode ShaderProgram::ParseShaderSourceCode()
{
	std::string shaderCode = ReadFile(filePath);
	std::stringstream ss(shaderCode);

	std::stringstream output[2];
	std::string line;
	int type = 0;

	while (getline(ss, line))
	{
		if (line.find("#vertex_shader") != std::string::npos) type = 0; // 0 means vertex
		else if (line.find("#fragment_shader") != std::string::npos) type = 1; // 1 means fragment
		else output[(int)type] << line << '\n';
	}

	return { output[0].str(), output[1].str() };
}

