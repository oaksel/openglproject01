#include "window.h"

bool GLFWIinitialized = false;

Window::Window(windowProperties wp)
{
	initializeGLFW(wp);
}
Window::~Window()
{
	terminate();
}

void Window::initializeGLEW()
{
	if (glewInit() != GLEW_OK) printf("ERROR::GLEW\nCould'nt start GLEW.");
	printf("GLEW VERSION: %s\n", glGetString(GL_VERSION));
}

void Window::onUpdate()
{
	glViewport(0, 0, properties.width, properties.height);
	glfwSwapBuffers(window);
	glfwPollEvents();
}
void Window::setVSync(bool enabled)
{
	if (enabled) glfwSwapInterval(1);
	else glfwSwapInterval(0);

	properties.VSync = enabled;
}

void Window::SetGLFWVersion(ushort major, ushort minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void Window::initializeGLFW(windowProperties wp)
{
	if (!GLFWIinitialized)
	{
		if (!glfwInit()) printf("ERROR::GLFW_INIT\n");
		GLFWIinitialized = true;

		SetGLFWVersion(3, 3);
	}

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	properties.title = wp.title;
	properties.width = wp.width;
	properties.height = wp.height;
	properties.VSync = wp.VSync;

	window = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		printf("ERROR::GLFW\nCreated window is invalid.\n");
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, &properties);

	initializeGLEW();
}
void Window::terminate()
{
	glfwDestroyWindow(window);
}

