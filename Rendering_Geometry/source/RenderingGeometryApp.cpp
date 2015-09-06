#include "RenderingGeometryApp.h"

bool RenderingGeometryApp::StartUp()
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

	const char* shader2 = "#version 330\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							uniform float time; \
							uniform float heightScale; \
							void main() \
							{ \
								vColour = Colour; \
								vec4 P = Position; \
								P.y += sin(time + Position.x) * heightScale; \
								gl_Position = ProjectionView * P;\
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
	//glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glShaderSource(vertexShader, 1, (const char**)&shader2, 0);
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


	GenerateGrid(ROWS, COLS);

	glUseProgram(mShaderProgramID);
	uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	uint timeUniform = glGetUniformLocation(mShaderProgramID, "time");
	uint heightScaleUniform = glGetUniformLocation(mShaderProgramID, "heightScale");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));
	glUniform1f(timeUniform, timer.DeltaTime);
	glUniform1f(heightScaleUniform, 0.0f);
	glBindVertexArray(mVAO);



	if (DEBUG_MODE)
	{
		int major = ogl_GetMajorVersion();
		int minor = ogl_GetMinorVersion();
		printf("GL: %i.%i\n", major, minor);
	}

	glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);
	glEnable(GL_DEPTH_TEST);

	//init model transforms

	return true;
}


void RenderingGeometryApp::ShutDown()
{
	delete mCamera;
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool RenderingGeometryApp::Update()
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

void RenderingGeometryApp::Draw()
{
	uint indexCount = (ROWS - 1) * (COLS - 1) * 6;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	uint projectionViewUniform = glGetUniformLocation(mShaderProgramID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void RenderingGeometryApp::InitCamera()
{
	mCamera = new FlyCamera(mWindow);
	mCamera->SetSpeed(10.0f);
	mCamera->SetRotationSpeed(10.0f);
	mCamera->SetPerspective(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
	mCamera->SetLookAt(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
}

void RenderingGeometryApp::GenerateGrid(uint rows, uint cols)
{
	Vertex* vertices = new Vertex[rows * cols];
	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			vertices[r*cols + c].color = vec4(color, 1);
		}
	}

	uint* indeces = new uint[(rows - 1) * (cols - 1) * 6];
	uint index = 0;
	for (uint r = 0; r < (rows - 1); r++)
	{
		for (uint c = 0; c < (cols - 1); c++)
		{
			//triangle 1
			indeces[index++] = r*cols + c;
			indeces[index++] = (r + 1)*cols + c;
			indeces[index++] = (r + 1)*cols + (c + 1);

			//triangle 2
			indeces[index++] = r*cols + c;
			indeces[index++] = (r + 1)*cols + (c + 1);
			indeces[index++] = r*cols + (c + 1);
		}
	}

	//create and bind buffers to a VAO
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);
	glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1)*(cols - 1) * 6 * sizeof(uint), indeces, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] indeces;
}