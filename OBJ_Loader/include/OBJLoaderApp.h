#pragma once
#include <vector>
#include <string>
#include "GameApp.h"
#include "FlyCamera.h"
#include "Keyboard.h"
#include "tiny_obj_loader\tiny_obj_loader.h"
#include "fbx_loader\FBXFile.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

typedef unsigned int uint;






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
	const char* OBJ_MODEL_FILE_PATH = "../OBJ_Loader/resources/models/bunny.obj";
	const char* FBX_MODEL_FILE_PATH = "../OBJ_Loader/resources/models/Bunny.fbx";

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

	struct Vertex
	{
		vec4 postion;
		vec4 color;
		vec4 normal;
		vec2 UV;
	};

	struct Geometry
	{
		std::vector<Vertex> vertices;
		std::vector<uint> indices;
	};


	FlyCamera* mCamera = nullptr;

	uint mShaderProgramID = 0;
	std::vector<GLInfo> mGLInfo;

	void InitCamera();
	bool LoadGeometry(const char* path);
	//Geometry& LoadGeometry(const char* path);
	bool LoadGLBuffers(OpenGLInfo& renderObject, const Geometry& geometry);
	//void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
	//void CreateOpenGLBuffers(FbxScene* scene);
	//void PrintFBXNode(FbxNode* node);
	//void PrintFBXAttribute(FbxNodeAttribute* attribute);
	//void PrintTabs();
	//FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	
};
