#include "Mouse.h"
#include <iostream>

double Mouse::posX = 0;
double Mouse::posY = 0;
//int Mouse::xDirection = 0;
//int Mouse::yDirection = 0;
double Mouse::scrollX = 0;
double Mouse::scrollY = 0;
Mouse::Cursor_Mode Mouse::mode = Cursor_Mode::NORMAL;
int Mouse::buttonState[3];

void Mouse::Init()
{
	glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouse_button_callback);
	glfwSetScrollCallback(glfwGetCurrentContext(), scroll_callback);
	glfwSetCursorPosCallback(glfwGetCurrentContext(), cursor_pos_callback);
}

void Mouse::Update(float deltaTime)
{

}

bool Mouse::IsButtonPressed(Button button)
{
	return buttonState[button] == GLFW_PRESS;
}

bool Mouse::IsButtonReleased(Button button)
{
	return buttonState[button] == GLFW_RELEASE;
}

void Mouse::SetMode(Cursor_Mode newMode)
{
	mode = newMode;
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, mode);
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	buttonState[button] = action;
}

void Mouse::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	posX = xpos;
	posY = ypos;
}