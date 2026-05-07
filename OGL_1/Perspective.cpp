#include "Perspective.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;



Perspective::Perspective() noexcept
	: dirty(true)
	, fovDeg(45.0f)
	, near(0.1f)
	, far(100.0f)
	, matrix(1.0f)
{
}

void Perspective::set(float fovDeg, float nearPlane, float farPlane, float aspectRatio)
{
	setFOV(fovDeg);
	setPlanes(nearPlane, farPlane);
	setAspectRatio(aspectRatio);
}

void Perspective::setFOV(float fovDeg)
{
	this->fovDeg = fovDeg;
	dirty = true;
}

void Perspective::setPlanes(float nearPlane, float farPlane)
{
	near = nearPlane;
	far = farPlane;
	dirty = true;
}

void Perspective::setAspectRatio(float ratio)
{
	aspectRatio = ratio;
	dirty = true;
}

const glm::mat4& Perspective::getMatrix()
{
	if (dirty)
	{
		evaluate();
	}

	return matrix;
}

float Perspective::getFOV() const
{
	return fovDeg;
}

float Perspective::getNearPlane() const
{
	return near;
}

float Perspective::getFarPlane() const
{
	return far;
}

float Perspective::getAspectRatio() const
{
	return aspectRatio;
}

void Perspective::evaluate()
{
	matrix = glm::perspective(glm::radians(fovDeg), aspectRatio, near, far);
	dirty = false;
}