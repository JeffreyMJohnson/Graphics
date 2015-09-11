#include "TexturesApp.h"

bool TexturesApp::StartUp()
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

	//std::string err = tinyobj::LoadObj(shapes, materials, MODEL_FILE_PATH);
	//if (err.length() != 0)
	//{
	//	std::cout << "Error loading OBJ file:\n" << err << std::endl;
	//	return false;
	//}



	//CreateOpenGLBuffers(shapes);
	
	InitCamera();

	// create shaders
	/*const char* vsSource = "#version 330\n \
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
	glDeleteShader(vertexShader);*/
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main() { \
							vTexCoord = TexCoord; \
							gl_Position= ProjectionView * Position;\
							}";
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { \
							FragColor = texture(diffuse,vTexCoord);\
							}";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	mShaderProgramID = glCreateProgram();
	glAttachShader(mShaderProgramID, vertexShader);
	glAttachShader(mShaderProgramID, fragmentShader);
	glLinkProgram(mShaderProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(mShaderProgramID);
	uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));

	//load texture data
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(CRATE_TEXTURE_PATH, &imageWidth, &imageHeight, &imageFormat, STBI_default);	if (data == nullptr)	{		std::cout << "error loading texture.\n";	}	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	CreateBuffers();

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

void TexturesApp::ShutDown()
{
	delete mCamera;
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool TexturesApp::Update()
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

void TexturesApp::Draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glUseProgram(mShaderProgramID);
	//uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	//glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));

	//for (uint i = 0; i < mGLInfo.size(); ++i)
	//{
	//	glBindVertexArray(mGLInfo[i].mVAO);
	//	glDrawElements(GL_TRIANGLES, mGLInfo[i].mIndexCount, GL_UNSIGNED_INT, 0);
	//}

	// use our texture program
	glUseProgram(mShaderProgramID);
	// bind the camera
	int loc = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));
	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	// tell the shader where it is
	loc = glGetUniformLocation(mShaderProgramID, "diffuse");
	glUniform1i(loc, 0);
	// draw
	glBindVertexArray(mGLInfo.mVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void TexturesApp::InitCamera()
{
	mCamera = new FlyCamera(mWindow);
	mCamera->SetSpeed(10.0f);
	mCamera->SetRotationSpeed(10.0f);
	mCamera->SetPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	mCamera->SetLookAt(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
}

void TexturesApp::CreateBuffers()
{
	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};
	glGenVertexArrays(1, &mGLInfo.mVAO);
	glBindVertexArray(mGLInfo.mVAO);
	glGenBuffers(1, &mGLInfo.mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
		vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &mGLInfo.mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
		indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}