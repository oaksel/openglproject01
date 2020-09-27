#include "Texture.h"

Texture::Texture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unBind();
	
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::setProperties(GLenum _format, int _width, int _height)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	width = _width;
	height = _height;
	format = _format;

	if (_format == GL_DEPTH_COMPONENT) dataType = GL_FLOAT;
	else if (_format == GL_RGBA) dataType = GL_UNSIGNED_BYTE;
	else dataType = GL_UNSIGNED_BYTE;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, dataType, NULL);

	unBind();
}

void Texture::loadFromFile(std::string path)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	pixels = stbi_load(path.c_str(), &width, &height, &channels, 4); // 4 means RGBA value 
	stbi_set_flip_vertically_on_load(1);

	format = GL_RGBA;
	dataType = GL_UNSIGNED_BYTE;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, dataType, pixels);

	unBind();
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + bindedSlot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::bind(int newSlot)
{
	bindedSlot = newSlot;
	bind();
}


void Texture::unBind()
{
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
