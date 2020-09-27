#pragma once
#include "prefix.h"


class Camera
{
private:

	glm::vec3 position	= glm::vec3(2.0f, 3.0f, 13.0f);

	glm::vec3 front		= glm::vec3(0.17f, 0.13f, -1.0f);
	glm::vec3 up		= glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right		= glm::cross(front, up);

	float angularVelocity = 0.006f; // for changing camera angle
	float linearVelocity = 15.0f; // for camera movement
	glm::dvec2 previousCursorPosition; // for camera movement

	float aspectRatio = 16.0f / 9.0f; // 16/9 ratio, 1.777778
	float fieldOfView = 60.0f, zFar = 160.0f;
	glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, zFar);

public:
	Camera() {}
	~Camera() {}

	void mouseUpdate(GLFWwindow * window);
	void keyboardUpdate(GLFWwindow * window, float deltaTime);
	inline void zoom(float level){ projection = glm::perspective(glm::radians(fieldOfView * 1 / level), aspectRatio, 0.1f, zFar); }

	inline glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }
	inline glm::mat4 getProjectionMatrix() { return projection; }
	inline glm::vec3 getCameraPosition() { return position; }
	inline void setCameraPosition(glm::vec3 newPosition) { position = newPosition; }

private:



};

