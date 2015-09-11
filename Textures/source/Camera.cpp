#include "Camera.h"

void Camera::Update(const float deltaTime)
{
}



void Camera::SetLookAt(const glm::vec3 from, const glm::vec3 to, const glm::vec3 up)
{
	mViewTransform = glm::lookAt(from, to, up);
	mWorldTransform = glm::inverse(mViewTransform);
}

void Camera::SetPosition(const glm::vec3 position)
{
	mWorldTransform = glm::translate(mWorldTransform, position);
	mViewTransform = glm::inverse(mWorldTransform);
	UpdateProjectViewTransform();
}

const glm::mat4 Camera::GetWorldTransform()
{
	return mWorldTransform;
}

const glm::mat4 Camera::GetView()
{
	return glm::inverse(mWorldTransform);
}

const glm::mat4 Camera::GetProjection()
{
	return mProjectionTransform;
}

void Camera::UpdateProjectViewTransform()
{
	mProjectionViewTransform = mProjectionTransform * mViewTransform;
}