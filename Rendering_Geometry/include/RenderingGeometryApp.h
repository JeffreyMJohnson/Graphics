#pragma once
#include "GameApp.h"
#include "FlyCamera.h"
#include "Keyboard.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

typedef unsigned int uint;

struct Vertex
{
	vec4 position;
	vec4 color;
};

class RenderingGeometryApp : public GameApp
{
public:
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Rendering Geometry";
	const vec4 CLEAR_COLOR = vec4(.25f, .25f, .25f, 1);
	const float CAMERA_FOV = glm::pi<float>() * .25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(10, 10, 10);
	const vec3 CAMERA_TO = vec3(0);
	const vec3 CAMERA_UP = vec3(0, 1, 0);

	const bool DEBUG_MODE = true;

	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();

private:
	FlyCamera* mCamera = nullptr;
	uint mVAO = 0;
	uint mVBO = 0;
	uint mIBO = 0;
	uint mShaderProgramID = 0;
	const uint ROWS = 25;
	const uint COLS = 25;
	void InitCamera();
	void GenerateGrid(uint rows, uint cols);
};
