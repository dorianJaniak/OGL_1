#include "Light.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

Light::Light(Type type, bool shadowActive, unsigned int shadowResolution)
	: ObjectTransformation()
	, visible(true)
	, type(type)
	, color(1.0f, 1.0f, 1.0f)
	, intensity(1.0f)
	, range(10.0f)
	, spotAngleDeg(30.0f)
	, spotInternalAngleDeg(20.0f)
	, shadowActive(shadowActive)
	, shadowMaxRes(shadowResolution)
	, shadowNearPlane(1.0f)
	, shadowFarPlane(100.0f)
{ }

void Light::setVisibility(bool visible)
{
	this->visible = visible;
}

void Light::setType(Type type)
{
	this->type = type;
}

void Light::setPosition(float x, float y, float z)
{
	setTranslation(x, y, z);
}

void Light::setDirection(float xPitchDeg, float yYawDeg)
{
	tmpXPitch = xPitchDeg;
	tmpYYaw = yYawDeg;

	// Mat4(1.0f) * RotY * RotX
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(yYawDeg), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(xPitchDeg), glm::vec3(1.0f, 0.0f, 0.0f));

	setRotation(rotation);
}

void Light::setShadowPlanes(float near, float far)
{
	shadowNearPlane = near;
	shadowFarPlane = far;
}

void Light::set(const glm::vec3& color, float intensity, float range)
{
	setColor(color);
	setIntensity(intensity);
	setRange(range);
}

void Light::setColor(const glm::vec3& color)
{
	this->color = color;
}

void Light::setIntensity(float intensity)
{
	this->intensity = intensity;
}

void Light::setRange(float range)
{
	this->range = range;
}

void Light::setSpotAngles(float externalAngle, float internalAngle)
{
	spotAngleDeg = externalAngle;
	spotInternalAngleDeg = internalAngle;
}

bool Light::isVisible() const
{
	return visible;
}

Light::Type Light::getType() const
{
	return type;
}

const glm::vec3& Light::getPosition() const
{
	return getTranslation();
}

const glm::mat3 Light::getRotation() const
{
	return ObjectTransformation::getRotation();
}

const glm::vec3 &Light::getColor() const
{
	return color;
}

float Light::getIntensity() const
{
	return intensity;
}

float Light::getRange() const
{
	return range;
}

float Light::getSpotlightAngle() const
{
	return spotAngleDeg;
}

float Light::getSpotlightInternalAngle() const
{
	return spotInternalAngleDeg;
}

glm::vec3 Light::getDirectionVector() const
{
	glm::vec4 direction = ObjectTransformation::getRotation() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	assert(direction.w >= 0.9999f && direction.w <= 1.0001f);
	return glm::vec3(direction);
}

bool Light::isShadowActive() const
{
	return shadowActive;
}

unsigned int Light::getShadowMaxResolution() const
{
	return shadowMaxRes;
}

float Light::getShadowNearPlane() const
{
	return shadowNearPlane;
}

float Light::getShadowFarPlane() const
{
	return shadowFarPlane;
}