#pragma once
#include "prefix.h"


 struct windowProperties
{
	std::string title = "empty";
	uint width = 640, height = 360;
	bool VSync = true;
};

class Window
{
private:
	windowProperties properties;

public:
	Window(windowProperties wp);
	~Window();

	GLFWwindow * window;

	void initializeGLEW();
	

	void onUpdate();
	void setVSync(bool enabled);

	inline bool IsVSync() { return properties.VSync; }

	inline void setWidth(int _width) { properties.width = _width; }
	inline void setHeight(int _height) { properties.height = _height; }
	inline int getWidth() { return properties.width; }
	inline int getHeight() { return properties.height; }


private:
	void SetGLFWVersion(ushort major, ushort minor);
	void initializeGLFW(windowProperties wp);
	void terminate();

};

