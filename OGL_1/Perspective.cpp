#include "Perspective.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;



Perspective::Perspective() noexcept
	: dirty(true)
	, fovDeg(45.0f)
	, near(0.1f)
	, far(100.0f)
	, aspectRatio(1.0f)
	, matrix(1.0f)
{
}

void Perspective::setPerspective(float fovDeg, float aspectRatio, float nearPlane, float farPlane)
{
	setFOV(fovDeg);
	setPlanes(nearPlane, farPlane);
	setAspectRatio(aspectRatio);
}

void Perspective::setFOV(float fovDeg)
{
	dirty |= (this->fovDeg != fovDeg);
	this->fovDeg = fovDeg;
}

void Perspective::setPlanes(float nearPlane, float farPlane)
{
	dirty |= (near != nearPlane || far != farPlane);
	near = nearPlane;
	far = farPlane;
}

void Perspective::setAspectRatio(float ratio)
{
	dirty |= (aspectRatio != ratio);
	aspectRatio = ratio;
}

const glm::mat4& Perspective::getPerspectiveMatrix()
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