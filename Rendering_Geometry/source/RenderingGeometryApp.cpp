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
	//mShader->LoadShader("../Rendering_Geometry/source/Simple_Vertex_Shader.glsl", "../Rendering_Geometry/source/Simple_Fragment_Shader.glsl");
	mShader->LoadShader("../Rendering_Geometry/source/Vertex_Shader_2.glsl", "../Rendering_Geometry/source/Simple_Fragment_Shader.glsl");


	GenerateGrid(ROWS, COLS);
	glBindVertexArray(mVAO);
	glUseProgram(mShader->GetProgram());
	mShader->SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));
	mShader->SetUniform("time", Shader::FLO1, &timer.DeltaTime);
	float height = .5f;
	mShader->SetUniform("heightScale", Shader::FLO1, &height);
	



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
	delete mShader;
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
	mShader->SetUniform("time", Shader::FLO1, &timer.CurrentTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	return true;
}

void RenderingGeometryApp::Draw()
{
	uint indexCount = (ROWS - 1) * (COLS - 1) * 6;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mShader->SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));
	/*uint projectionViewUniform = glGetUniformLocation(mShader->GetProgram(), "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mCamera->GetProjection() * mCamera->GetView()));*/
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
	uint verticesSize = rows * cols;
	Vertex* vertices = new Vertex[verticesSize];
	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			vertices[r*cols + c].color = vec4(color, 1);
		}
	}
	uint indecesCount = (rows - 1) * (cols - 1) * 6;
	uint* indeces = new uint[indecesCount];
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
	LoadGLBuffer(vertices, verticesSize, indeces, indecesCount);

	delete[] vertices;
	delete[] indeces;
}

void RenderingGeometryApp::LoadGLBuffer(Vertex* vertices, uint verticesSize, uint* indeces, uint indecesCount)
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecesCount * sizeof(uint), indeces, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
