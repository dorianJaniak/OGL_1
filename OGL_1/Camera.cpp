#include "Camera.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

Camera::Camera()
	: Perspective()
	, dirtyView(true)
	, position(0.0f, 0.0f, 0.0f)
	, rotation(0.0f, 0.0f, 0.0f)
	, matrix(1.0f)
{
}

Camera::Camera(const Camera& cam)
	: dirtyView(cam.dirtyView)
	, position(cam.position)
	, rotation(cam.rotation)
	, matrix(cam.matrix)
{
}

void Camera::updateView()
{
	updateRotationMatrix();
	matrix = glm::translate(rotationMatrix, -position);
	dirtyView = false;
}

void Camera::setPosition(const glm::vec3& pos)
{
	position = pos;
	dirtyView = true;
}

void Camera::setRotation(const glm::vec3& rot)
{
	rotation = rot;
	dirtyView = true;
}

void Camera::fly(const glm::vec3& move)
{
	updateRotationMatrix();
	
	//position += glm::vec3(glm::transpose(rotationMatrix) * glm::vec4(-move, 1.0f));
	// Transpose is required because rotationMatrix is the matrix ready for world transformation
	position += glm::vec3(glm::transpose(rotationMatrix) * glm::vec4(move, 1.0f));
	dirtyView = true;
}

void Camera::lookSide(float degree)
{
	rotation.y += degree;
	dirtyView = true;
}

void Camera::lookVerticaly(float degree)
{
	rotation.x += degree;
	dirtyView = true;
}

glm::mat4 Camera::getViewMatrix()
{
	if (dirtyView)
	{
		updateView();
		dirtyView = false;
	}

	return matrix;
}

glm::mat4 Camera::getVPMatrix()
{
	return Perspective::getPerspectiveMatrix() * getViewMatrix();
}

glm::mat4 Camera::getSkyboxMatrix()
{
	return Perspective::getPerspectiveMatrix() * rotationMatrix;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

void Camera::updateRotationMatrix()
{
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
}

