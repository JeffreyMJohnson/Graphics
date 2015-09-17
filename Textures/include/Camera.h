#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
public:
	void Update(const float deltaTime);
	void SetLookAt(const glm::vec3 from, const glm::vec3 to, const glm::vec3 up);
	void SetPosition(const glm::vec3 position);
	const glm::mat4 GetWorldTransform();
	const glm::mat4 GetView();
	const glm::mat4 GetProjection();


protected:
	glm::mat4 mWorldTransform = glm::mat4();//logical update
	glm::mat4 mViewTransform = glm::mat4();//draw transform
	glm::mat4 mProjectionTransform = glm::mat4();
	glm::mat4 mProjectionViewTransform = glm::mat4(); //projectionview * projectionTransform - draw transform
	void UpdateProjectViewTransform();
};