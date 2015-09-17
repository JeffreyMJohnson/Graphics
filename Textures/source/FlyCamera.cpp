#include "FlyCamera.h"


FlyCamera::FlyCamera(GLFWwindow* window)
{
	mWindow = window;
	Mouse::Init();
	Mouse::SetMode(Mouse::Cursor_Mode::DISABLED);
	Keyboard::Init();
}

void FlyCamera::SetPerspective(const float fov, const float aspectRatio, const float near, const float far)
{
	mProjectionTransform = glm::perspective(fov, aspectRatio, near, far);
	UpdateProjectViewTransform();
}

void FlyCamera::SetRotationSpeed(const float rotSpeed)
{
	mRotSpeed = rotSpeed;
}


void FlyCamera::SetSpeed(const float speed)
{
	mSpeed = speed;
}

void FlyCamera::Rotate(float angle, glm::vec3 axis)
{
	mWorldTransform = glm::rotate(mWorldTransform, angle, axis);
	mViewTransform = glm::inverse(mWorldTransform);
	UpdateProjectViewTransform();
}

void FlyCamera::Translate(glm::vec3 distance)
{
	mWorldTransform = glm::translate(mWorldTransform, distance);
	mViewTransform = glm::inverse(mWorldTransform);
	UpdateProjectViewTransform();
}

void FlyCamera::Update(float deltaTime)
{
	Mouse::Update();
	glm::vec3 direction = glm::vec3(0);
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		direction = glm::vec3(0, 0, -1);
	}
	else if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		direction = glm::vec3(0, 0, 1);
	}
	else if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		direction = glm::vec3(-1, 0, 0);
	}
	else if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		direction = glm::vec3(1, 0, 0);
	}

	Translate(deltaTime * mSpeed * direction);

	int deltaX = Mouse::GetPosX() - Mouse::GetPrevPosX();
	//std::cout << "directionX: " << Mouse::GetDirectionX() << " Prev: " << Mouse::GetPrevPosX() << " Current: " << Mouse::GetPosX() << std::endl;

	if (Mouse::IsButtonPressed(Mouse::LEFT))
	{
		//double sensitivity = .005;
		pitch = 0; //rotate around x axis
		yaw = 0; //rotate around y axis

		//calc mouse offset 
		double deltaX = Mouse::GetPosX() - Mouse::GetPrevPosX();
		double deltaY = Mouse::GetPosY() - Mouse::GetPrevPosY();//reversed because y is upper in gl

		//add offset to yaw and pitch values
		yaw += deltaX;
		pitch += deltaY;


		//constrain view to prevent hijinks
		if (pitch > 89.0)
		{
			pitch = 89.0;
		}
		if (pitch < -89.0)
		{
			pitch = -89.0;
		}
		
		Rotate(yaw * sensitivity, glm::vec3(0, 1, 0));
		Rotate(pitch * sensitivity, glm::vec3(1, 0, 0));

	}
	else
	{
		mCursorXPos = -1;
		pitch = 0;
		yaw = 0;
	}
}