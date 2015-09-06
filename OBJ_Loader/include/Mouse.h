#pragma once
#include <GLFW\glfw3.h>


class Mouse
{
public:
	enum Mouse_Button
	{
		LEFT,
		RIGHT,
		CENTER
	};

	static void Init();

	static void Update();

	static bool IsButtonPressed(Mouse_Button button);

	static double GetPosX() { return posX; }
	static double GetPosY() { return posY; }
	static double GetPrevPosX() { return prevPosX; }
	static double GetPrevPosY() { return prevPosY; }
	static double GetPosDeltaX() { return posDeltaX; }
	static double GetPosDeltaY() { return posDeltaY; }
	static double GetScrollX() { return scrollX; }
	static double GetScrollY() { return scrollY; }

private:
	static double scrollX, scrollY;
	static double posX, posY;
	static double prevPosX, prevPosY;
	static double posDeltaX, posDeltaY;
	static int GLFWButtonCodes[3];

	static void scroll_callback(GLFWwindow *w, double x, double y) { scrollX = x; scrollY = y; }
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
	{
		posX = xpos;
		posY = ypos;
	}

	


};