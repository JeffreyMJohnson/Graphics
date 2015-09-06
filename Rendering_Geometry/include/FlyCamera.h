#pragma once
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <GLFW\glfw3.h>
#include <iostream>

class FlyCamera : public Camera
{
public:
	FlyCamera(GLFWwindow* window);
	void SetPerspective(const float fov, const float aspectRatio, const float near, const float far);
	void SetSpeed(const float speed);
	void SetRotationSpeed(const float rotSpeed);
	void Rotate(float angle, glm::vec3 axis);
	void Translate(glm::vec3 distance);
	void Update(float deltaTime);
	void Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset);
private:
	float mSpeed = 1.0f;
	float mRotSpeed = 1.0f;
	glm::vec3 mUp;
	GLFWwindow* mWindow = nullptr;
	double mCursorXPos = 0;
	double mCursorYPos = 0;

	void HandleInput(float deltaTime);

};