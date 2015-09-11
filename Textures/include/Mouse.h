#pragma once
#include <GLFW\glfw3.h>


class Mouse
{
public:
	enum Button
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

	/*Mode to set the hotdog
	NORMAL = The regular arrow cursor is used and cursor motion is not limited. (Default)
	DISABLED = This will hide the cursor and lock it to the specified window. GLFW will 
			   then take care of all the details of cursor re-centering and offset calculation and providing the application with a virtual cursor position.
	HIDDEN = If you just wish the cursor to become hidden when it is over a window.
	*/
	enum Cursor_Mode
	{
		NORMAL,
		DISABLED,
		HIDDEN
	};

	static void Init();

	static void Update();

	static bool IsButtonPressed(Button button);
	static bool IsButtonReleased(Button button);

	static double GetPosX() { return posX; }
	static double GetPosY() { return posY; }
	static double GetPrevPosX() { return prevPosX; }
	static double GetPrevPosY() { return prevPosY; }
	static double GetScrollX() { return scrollX; }
	static double GetScrollY() { return scrollY; }
	static int GetDirectionX() { return directionX; }
	static Cursor_Mode GetMode() { return mode; }

	static void SetMode(Cursor_Mode newMode);

private:
	static double scrollX, scrollY;
	static double posX, posY;
	static double prevPosX, prevPosY;
	static int directionX, directionY;
	static Cursor_Mode mode;
	static int buttonState[];
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow *w, double x, double y) { scrollX = x; scrollY = y; }
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static bool isFirstMouse;



};