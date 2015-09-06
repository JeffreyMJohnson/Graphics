#include "Mouse.h"

double Mouse::posX = 0;
double Mouse::posY = 0;
double Mouse::prevPosX = 0;
double Mouse::prevPosY = 0;
double Mouse::posDeltaX = 0;
double Mouse::posDeltaY = 0;
double Mouse::scrollX = 0;
double Mouse::scrollY = 0;
int Mouse::GLFWButtonCodes[3] = { GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE };

void Mouse::Init()
{
	glfwSetScrollCallback(glfwGetCurrentContext(), scroll_callback);
	glfwSetCursorPosCallback(glfwGetCurrentContext(), cursor_pos_callback);
}

void Mouse::Update()
{
	prevPosX = posX;
	prevPosY = posY;
	glfwGetCursorPos(glfwGetCurrentContext(), &posX, &posY);
	posDeltaX = posX - prevPosX;
	posDeltaY = posY - prevPosY;
}

bool Mouse::IsButtonPressed(Mouse_Button button)
{
	return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
}