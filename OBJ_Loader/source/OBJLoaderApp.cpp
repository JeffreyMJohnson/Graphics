#include "OBJLoaderApp.h"

bool OBJLoaderApp::StartUp()
{

	if (!glfwInit())
	{
		return false;
	}

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	if (nullptr == mWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
		return false;
	}

	//load model file
	std::string err = tinyobj::LoadObj(shapes, materials, MODEL_FILE_PATH);
	if (err.length() != 0)
	{
		std::cout << "Error loading OBJ file:\n" << err << std::endl;
		return false;
	}

	CreateOpenGLBuffers(shapes);

	InitCamera();

	// create shaders
	const char* vsSource = "#version 330\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							void main() \
							{ \
								vColour = Colour; \
								gl_Position = ProjectionView * Position;\
							}";

	const char* fsSource = "#version 330\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() \
							{\
								FragColor = vColour;\
							}";

	int success = GL_FALSE;
	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	mShaderProgramID = glCreateProgram();
	glAttachShader(mShaderProgramID, vertexShader);
	glAttachShader(mShaderProgramID, fragmentShader);
	glLinkProgram(mShaderProgramID);

	glGetProgramiv(mShaderProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(mShaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(mShaderProgramID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glUseProgram(mShaderProgramID);
	uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));



	if (DEBUG_MODE)
	{
		int major = ogl_GetMajorVersion();
		int minor = ogl_GetMinorVersion();
		printf("GL: %i.%i\n", major, minor);
	}

	glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}


void OBJLoaderApp::ShutDown()
{
	delete mCamera;
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool OBJLoaderApp::Update()
{
	if (glfwWindowShouldClose(mWindow) || glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}
	timer.Update(glfwGetTime());
	mCamera->Update(timer.DeltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	return true;
}

void OBJLoaderApp::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(mShaderProgramID);
	uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));

	for (uint i = 0; i < mGLInfo.size(); ++i)
	{
		glBindVertexArray(mGLInfo[i].mVAO);
		glDrawElements(GL_TRIANGLES, mGLInfo[i].mIndexCount, GL_UNSIGNED_INT, 0);
	}
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void OBJLoaderApp::InitCamera()
{
	mCamera = new FlyCamera(mWindow);
	mCamera->SetSpeed(10.0f);
	mCamera->SetRotationSpeed(10.0f);
	mCamera->SetPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	mCamera->SetLookAt(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
}


void OBJLoaderApp::CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	mGLInfo.resize(shapes.size());

	for (uint meshIndex = 0; meshIndex < shapes.size(); ++meshIndex)
	{
		glGenVertexArrays(1, &mGLInfo[meshIndex].mVAO);
		glGenBuffers(1, &mGLInfo[meshIndex].mVBO);
		glGenBuffers(1, &mGLInfo[meshIndex].mIBO);
		glBindVertexArray(mGLInfo[meshIndex].mVAO);

		uint floatCount = shapes[meshIndex].mesh.positions.size();
		floatCount += shapes[meshIndex].mesh.normals.size();
		floatCount += shapes[meshIndex].mesh.texcoords.size();

		std::vector<float> vertexData;
		vertexData.reserve(floatCount);

		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.positions.begin(), shapes[meshIndex].mesh.positions.end());

		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.normals.begin(), shapes[meshIndex].mesh.normals.end());

		mGLInfo[meshIndex].mIndexCount = shapes[meshIndex].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, mGLInfo[meshIndex].mVBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo[meshIndex].mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[meshIndex].mesh.indices.size() * sizeof(uint), shapes[meshIndex].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);//position
		glEnableVertexAttribArray(1);//normal data

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[meshIndex].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}