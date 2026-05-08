#include "CameraCube.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

CameraCube::CameraCube() noexcept
	: Perspective()
	, dirtyTransformations(true)
	, position(0.0f, 0.0f, 0.0f)
	, transformations({1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f })
	, defaultMat(1.0f)
{
	Perspective::setAspectRatio(1.0f);
	Perspective::setFOV(90.0f);
	Perspective::setPlanes(0.1f, 25.0f);
}

// Set position in world space
// When pos.z < 0, then camera moves forward (world moves backward)
// When pos.z > 0, then camera moves backward (world moves forward)
void CameraCube::setPosition(const glm::vec3& pos)
{
	dirtyTransformations = true;
	position = pos;
}

void CameraCube::setPlanes(float near, float far)
{
	Perspective::setPlanes(near, far);
}

const glm::mat4& CameraCube::getPerspectiveMatrix()
{
	return Perspective::getPerspectiveMatrix();
}

const glm::mat4& CameraCube::getVPMatrix(unsigned int side)
{
	update();

	if (side > 5u)
	{
		return defaultMat;
	}

	return transformations[side];
}

const std::array<glm::mat4, 6>& CameraCube::getVPMatrices()
{
	update();
	return transformations;
}

float CameraCube::getNear() const
{
	return Perspective::getNearPlane();
}

float CameraCube::getFar() const
{
	return Perspective::getFarPlane();
}

const glm::vec3& CameraCube::getPosition() const
{
	return position;
}

void CameraCube::update()
{
	Perspective::getPerspectiveMatrix();

	if (dirtyTransformations)
	{
		updateTransformations();
		dirtyTransformations = false;
	}
}

void CameraCube::updateTransformations()
{
	static constexpr glm::vec3 dirPoints[6] = {
		{1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f},
	};

	static constexpr glm::vec3 dirTop[6] = {
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
	};

	for (unsigned int i = 0; i < 6u; ++i)
	{
		transformations[i] = Perspective::getPerspectiveMatrix() * glm::lookAt(position, position + dirPoints[i], dirTop[i]);
	}
}