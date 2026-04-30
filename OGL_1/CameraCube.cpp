#include "CameraCube.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

CameraCube::CameraCube()
	: dirtyPerspective(true)
	, dirtyTransformations(true)
	, near(0.1f)
	, far(25.0f)
	, position(0.0f, 0.0f, 0.0f)
	, perspectiveMatrix(1.0f)
	, transformations({1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f })
	, defaultMat(1.0f)
{
}

CameraCube::CameraCube(const CameraCube& camera)
	: dirtyPerspective(camera.dirtyPerspective)
	, dirtyTransformations(camera.dirtyTransformations)
	, near(camera.near)
	, far(camera.far)
	, position(camera.position)
	, perspectiveMatrix(camera.perspectiveMatrix)
	, transformations(camera.transformations)
	, defaultMat(1.0f)
{
}

// Set position in world space
// When pos.z < 0, then camera moves forward (world moves backward)
// When pos.z > 0, then camera moves backward (world moves forward)
void CameraCube::setPosition(const glm::vec3& pos)
{
	dirtyTransformations = true;
	position = pos;
}

void CameraCube::setPerspective(float near, float far)
{
	setPlanes(near, far);
}

void CameraCube::setPlanes(float near, float far)
{
	dirtyPerspective = true;
	this->near = near;
	this->far = far;
}

const glm::mat4& CameraCube::getPerspectiveMatrix()
{
	if (dirtyPerspective)
	{
		updatePerspective();
		dirtyPerspective = false;
	}

	return perspectiveMatrix;
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
	return near;
}

float CameraCube::getFar() const
{
	return far;
}

const glm::vec3& CameraCube::getPosition() const
{
	return position;
}

void CameraCube::update()
{
	if (dirtyPerspective)
	{
		updatePerspective();
		dirtyPerspective = false;
	}

	if (dirtyTransformations)
	{
		updateTransformations();
		dirtyTransformations = false;
	}
}

void CameraCube::updatePerspective()
{
	perspectiveMatrix = glm::perspective(glm::radians(90.0f), 1.0f, near, far);
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
		transformations[i] = perspectiveMatrix * glm::lookAt(position, position + dirPoints[i], dirTop[i]);
	}
}