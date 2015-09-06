#pragma once
#include <vector>
#include <string>
#include "GameApp.h"
#include "FlyCamera.h"
#include "Keyboard.h"
#include "tiny_obj_loader\tiny_obj_loader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

typedef unsigned int uint;

struct Vertex
{
	vec4 position;
	vec4 color;
};

class OBJLoaderApp : public GameApp
{
public:
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "OBJ Loader";
	const vec4 CLEAR_COLOR = vec4(.25f, .25f, .25f, 1);
	const float CAMERA_FOV = glm::pi<float>() * .25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(10, 10, 10);
	const vec3 CAMERA_TO = vec3(0);
	const vec3 CAMERA_UP = vec3(0, 1, 0);
	const char* MODEL_FILE_PATH = "../OBJ_Loader/resources/models/bunny.obj";

	const bool DEBUG_MODE = true;

	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();

private:
	typedef struct OpenGLInfo
	{
		uint mVAO;
		uint mVBO;
		uint mIBO;
		uint mIndexCount;
	} GLInfo;

	FlyCamera* mCamera = nullptr;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	uint mShaderProgramID = 0;
	std::vector<GLInfo> mGLInfo;
	const uint ROWS = 25;
	const uint COLS = 25;
	void InitCamera();
	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
};
#pragma once
