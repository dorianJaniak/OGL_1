#include "Camera.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

Camera::Camera()
	: dirtyView(true)
	, dirtyPerspective(true)
	, position(0.0f, 0.0f, 0.0f)
	, rotation(0.0f, 0.0f, 0.0f)
	, matrix(1.0f)
	, perspectiveMatrix(1.0f)
	, near(0.1f)
	, far(100.0f)
	, fovDeg(30.0f)
	, aspectRatio(1.0f)
{
}

Camera::Camera(const Camera& cam)
	: dirtyView(cam.dirtyView)
	, dirtyPerspective(cam.dirtyPerspective)
	, position(cam.position)
	, rotation(cam.rotation)
	, matrix(cam.matrix)
	, perspectiveMatrix(cam.perspectiveMatrix)
	, near(cam.near)
	, far(cam.far)
	, fovDeg(cam.fovDeg)
	, aspectRatio(cam.aspectRatio)
{
}

void Camera::updateView()
{
	updateRotationMatrix();
	matrix = glm::translate(rotationMatrix, -position);
	dirtyView = false;
}

void Camera::updatePerspective()
{
	perspectiveMatrix = glm::perspective(glm::radians(fovDeg), aspectRatio, near, far);
	dirtyPerspective = false;
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

void Camera::setPerspective(float fovDeg, float aspectRatio, float near, float far)
{
	this->fovDeg = fovDeg;
	this->aspectRatio = aspectRatio;
	this->near = near;
	this->far = far;
	dirtyPerspective = true;
}

void Camera::setFov(float fovDeg)
{
	dirtyPerspective = (this->fovDeg != fovDeg);
	this->fovDeg = fovDeg;
}

void Camera::setAspectRatio(float aspectRatio)
{
	dirtyPerspective = (this->aspectRatio != aspectRatio);
	this->aspectRatio = aspectRatio;
}

void Camera::setPlanes(float near, float far)
{
	dirtyPerspective = (this->near != near || this->far != far);
	this->near = near;
	this->far = far;
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

const glm::mat4 &Camera::getPerspectiveMatrix()
{
	if (dirtyPerspective)
	{
		updatePerspective();
		dirtyPerspective = false;
	}

	return perspectiveMatrix;
}

glm::mat4 Camera::getVPMatrix()
{
	return getPerspectiveMatrix() * getViewMatrix();
}

glm::mat4 Camera::getSkyboxMatrix()
{
	return getPerspectiveMatrix() * rotationMatrix;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

float Camera::getFov() const
{
	return fovDeg;
}

float Camera::getNear() const
{
	return near;
}

float Camera::getFar() const
{
	return far;
}

void Camera::updateRotationMatrix()
{
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
}

