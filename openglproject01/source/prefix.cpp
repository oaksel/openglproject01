#include "prefix.h"

std::string ReadFile(std::string filePath)
{
	std::ifstream reading(filePath);
	std::stringstream output;
	std::string line;

	if (!reading)
	{
		printf("ERROR::READ_FILE\nFile could not found at\n%s\n", filePath.c_str());
		return "";
	}

	while (getline(reading, line)) output << line << '\n';

	return output.str();
}
uint GLenumSize(GLenum e)
{
	switch (e)
	{
	case GL_FLOAT: return sizeof(GLfloat); break;
	case GL_UNSIGNED_SHORT: return sizeof(GLushort); break;
	case GL_UNSIGNED_INT: return sizeof(GLuint); break;
	default: break;
	}
}


void enableBlend()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}
