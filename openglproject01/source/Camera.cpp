#include "Camera.h"

void Camera::mouseUpdate(GLFWwindow * window)
{

	#define buttonPressed(x) glfwGetMouseButton(window, x) == GLFW_PRESS
	#define buttonReleased(x) glfwGetMouseButton(window, x) == GLFW_RELEASE

	bool move = false;
	if (buttonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		
		glm::dvec2 currentCursorPosition;
		glfwGetCursorPos(window, &currentCursorPosition.x, &currentCursorPosition.y);

		glm::dvec2 cursorDelta = previousCursorPosition - currentCursorPosition;

		if (glm::length(cursorDelta) > 50.0f)
		{
			previousCursorPosition = currentCursorPosition;
			return;
		}
		previousCursorPosition = currentCursorPosition;

		glm::vec4 front4 = glm::vec4(front, 1.0f);
		front4 = glm::rotate((float)cursorDelta.x * angularVelocity, up) * front4;

		right = glm::cross(front, up); // maybe use a temp_right variable :-/
		front4 = glm::rotate((float)cursorDelta.y * angularVelocity, right) * front4;


		front = glm::vec3(front4);

		if (front.y > 0.9f) front.y = 0.9f;
		if (front.y < -0.9f) front.y = -0.9f;


		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, up));

	}
	else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	
}

void Camera::keyboardUpdate(GLFWwindow * window, float deltaTime)
{
	#define keyPressed(x) glfwGetKey(window, x) == GLFW_PRESS
	#define keyReleased(x) glfwGetKey(window, x) == GLFW_RELEASE
	
	if (keyPressed(GLFW_KEY_W))			position += front * linearVelocity * deltaTime;
	else if (keyPressed(GLFW_KEY_S))	position -= front * linearVelocity * deltaTime;
	if (keyPressed(GLFW_KEY_A))			position -= right * linearVelocity * deltaTime;
	else if (keyPressed(GLFW_KEY_D))	position += right * linearVelocity * deltaTime;

	if (keyPressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);
}

