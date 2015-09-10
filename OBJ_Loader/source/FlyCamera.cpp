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
		float angleY = 0;

		if (mCursorXPos == -1)
		{
			mCursorXPos = Mouse::GetPosX();
		}
		angleY = mCursorXPos - Mouse::GetPosX();


		Rotate(glm::radians(deltaTime * angleY), glm::vec3(0, 1, 0));
		angleY = 0;
		//Rotate(glm::radians(deltaTime * mRotSpeed * Mouse::GetYDirection()), glm::vec3(1, 0, 0));
		using namespace std;
		//cout << "deltaX: " << Mouse::GetPosDeltaX() << endl;

		//int xDirection = 0;
		//int yDirection = 0;
		//if(Mouse::GetPosDeltaX() >)

		//if ((int)Mouse::GetPosDeltaX() != 0)
		//{
		//	
		//}
		//if ((int)Mouse::GetPosDeltaY() != 0)
		//{
		//	Rotate(glm::radians(deltaTime * mRotSpeed * Mouse::GetPosDeltaY()), glm::vec3(1, 0, 0));
		//}

	}
	else
	{
		mCursorXPos = -1;
	}
	//std::cout << "W: " << Keyboard::IsKeyPressed(Keyboard::KEY_W) << std::endl;
	//std::cout << "x: " << Mouse::GetPrevPosX() << std::endl;
}