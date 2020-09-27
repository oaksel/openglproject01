#pragma once
#include "prefix.h"
#include "stb_image/stb_image.h"

class Texture
{
private:
	uint textureID;
	std::string filePath;
	int height = 0, width = 0;
	int channels = 0;
	GLenum format;
	GLenum dataType;
	void *pixels;

	int bindedSlot = 0;

public:
	Texture();
	~Texture();

	inline uint getID() { return textureID; }
	void setProperties(GLenum _format, int _width, int _height);

	void loadFromFile(std::string path);

	void bind();
	void bind(int newSlot);
	void unBind();

	inline int getBindedSlot() { return bindedSlot; }
	inline void *getPixels() { return pixels; }
	inline void setPixels(void *_pixels) { pixels = _pixels; }
};

