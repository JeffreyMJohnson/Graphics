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
	//LoadGeometry(OBJ_MODEL_FILE_PATH);
	LoadGeometry(FBX_MODEL_FILE_PATH);

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

bool OBJLoaderApp::LoadGeometry(const char * path)
{
	bool success = true;
	//find extension
	std::string sPath(path);
	std::string ext = sPath.substr(sPath.find_last_of('.'));

	Geometry geometry;

	if (ext == ".obj")
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, path);
		if (err.length() != 0)
		{
			std::cout << "Error loading OBJ file:\n" << err << std::endl;
			success = false;
		}

		//hard coding only using first shape, can change to loop here
		if (success)
		{
			auto shape = shapes[0];
			auto mesh = shape.mesh;

			geometry.vertices.resize(mesh.positions.size());

			uint posIndex = 0;
			uint normalIndex = 0;
			uint UVIndex = 0;
			bool hasNormals = mesh.normals.size() == mesh.positions.size();
			bool hasUVs = mesh.texcoords.size() == mesh.positions.size();
			//obj has vectors of floats, my struct and shaders uses glm vecs so need to build myself
			for (uint vertexCount = 0; posIndex < mesh.positions.size(); vertexCount++)
			{
				float x = mesh.positions[posIndex++];
				float y = mesh.positions[posIndex++];
				float z = mesh.positions[posIndex++];
				geometry.vertices[vertexCount].postion = vec4(x, y, z, 1);

				if (hasNormals)
				{
					x = mesh.normals[normalIndex++];
					y = mesh.normals[normalIndex++];
					z = mesh.normals[normalIndex++];
					geometry.vertices[vertexCount].normal = vec4(x, y, z, 1);
				}

				if (hasUVs)
				{
					x = mesh.texcoords[UVIndex++];
					y = mesh.texcoords[UVIndex++];
					geometry.vertices[vertexCount].UV = vec2(x, y);
				}
			}

			geometry.indices = mesh.indices;
		}
	}
	else if (ext == ".fbx")
	{


		FBXFile file;
		success = file.load(path, FBXFile::UNITS_METER, false, false, false);
		if (!success)
		{
			std::cout << "Error loading FBX file:\n";
		}
		else
		{
			//hardcoding to use single mesh, can loop here if needed.
			FBXMeshNode* mesh = file.getMeshByIndex(0);
			geometry.vertices.resize(mesh->m_vertices.size());

			for (int i = 0; i < mesh->m_vertices.size();i++)
			{
				auto xVert = mesh->m_vertices[i];
				geometry.vertices[i].postion = xVert.position;
				geometry.vertices[i].color = xVert.colour;
				geometry.vertices[i].normal = xVert.normal;
				geometry.vertices[i].UV = xVert.texCoord1;
			}

			geometry.indices = mesh->m_indices;

			file.unload();
		}

	}
	else
	{
		std::cout << "Unsupported format. Only support .obj or .fbx files.\n";
		success = false;
	}
	if (!success)
	{
		return false;
	}

	GLInfo renderObject;
	LoadGLBuffers(renderObject, geometry);
	mGLInfo.push_back(renderObject);

	return true;
}

bool OBJLoaderApp::LoadGLBuffers(OpenGLInfo & renderObject, const Geometry & geometry)
{
	glGenVertexArrays(1, &renderObject.mVAO);
	glGenBuffers(1, &renderObject.mVBO);
	glGenBuffers(1, &renderObject.mIBO);

	glBindVertexArray(renderObject.mVAO);

	//uint floatCount = shapes[meshIndex].mesh.positions.size();
	//floatCount += shapes[meshIndex].mesh.normals.size();
	//floatCount += shapes[meshIndex].mesh.texcoords.size();

	renderObject.mIndexCount = geometry.indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, renderObject.mVBO);
	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderObject.mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(uint), geometry.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//color in shader right now.
	//glEnableVertexAttribArray(2);//normal
	//glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//THIS NEEDS TO BE CHANGED WHEN SHADER IS UPDATED
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));// 1));
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));
	//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

/*
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
*/
//void OBJLoaderApp::CreateOpenGLBuffers(FbxScene* scene)
//{
//	FbxNode* rootNode = scene->GetRootNode();
//	mGLInfo.resize(rootNode->GetChildCount());
//
//	
//
//	//for (uint meshIndex = 0; meshIndex < shapes.size(); ++meshIndex)
//	for (uint meshIndex = 0; meshIndex < rootNode->GetChildCount(); ++meshIndex)
//	{
//		glGenVertexArrays(1, &mGLInfo[meshIndex].mVAO);
//		glGenBuffers(1, &mGLInfo[meshIndex].mVBO);
//		glGenBuffers(1, &mGLInfo[meshIndex].mIBO);
//		glBindVertexArray(mGLInfo[meshIndex].mVAO);
//
//		FbxMesh* mesh = rootNode->GetChild(meshIndex)->GetMesh();
//
//		uint floatCount = mesh->mPolygonVertices.Size();
//			//shapes[meshIndex].mesh.positions.size();
//		floatCount += mesh->norm
//			//shapes[meshIndex].mesh.normals.size();
//		floatCount += shapes[meshIndex].mesh.texcoords.size();
//
//
//		std::vector<float> vertexData;
//		vertexData.reserve(floatCount);
//
//		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.positions.begin(), shapes[meshIndex].mesh.positions.end());
//
//		vertexData.insert(vertexData.end(), shapes[meshIndex].mesh.normals.begin(), shapes[meshIndex].mesh.normals.end());
//
//		mGLInfo[meshIndex].mIndexCount = shapes[meshIndex].mesh.indices.size();
//
//		glBindBuffer(GL_ARRAY_BUFFER, mGLInfo[meshIndex].mVBO);
//		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo[meshIndex].mIBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[meshIndex].mesh.indices.size() * sizeof(uint), shapes[meshIndex].mesh.indices.data(), GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0);//position
//		glEnableVertexAttribArray(1);//normal data
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[meshIndex].mesh.positions.size()));
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//}



/*
void OBJLoaderApp::PrintFBXNode(FbxNode* node)
{
	using namespace std;
	const char* name = node->GetName();
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	// Print the contents of the node
	printf("<node name='%s' translation='(%f,%f,%f)' rotation='(%f,%f,%f)' scaling='(%f,%f,%f)'>\n",
		name,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]
		);

	// Print the node's attributes.
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
		PrintFBXAttribute(node->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < node->GetChildCount(); j++)
		PrintFBXNode(node->GetChild(j));

	mNumTabs--;
	PrintTabs();
	printf("</node>\n");

}

void OBJLoaderApp::PrintFBXAttribute(FbxNodeAttribute* attribute)
{
	if (!attribute) return;

	FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
	FbxString attrName = attribute->GetName();
	PrintTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());

}

void OBJLoaderApp::PrintTabs()
{
	for (int i = 0; i < mNumTabs; i++)
		printf("\t");
}

FbxString OBJLoaderApp::GetAttributeTypeName(FbxNodeAttribute::EType type) {


	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}
*/