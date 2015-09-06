#pragma once
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\transform.hpp>
#include "Timer.h"
#include "Camera.h"


class GameApp
{
public:
	virtual bool StartUp() = 0;
	virtual void ShutDown() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;

	GLFWwindow* mWindow = nullptr;
	Timer timer = Timer();
	
};