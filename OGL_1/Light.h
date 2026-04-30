#pragma once
#include "ObjectTransformation.h"
#include <glm.hpp>

namespace dj {

/*!
	\todo Add Near and Far plane (?) for RenderShadedWorldNode (u_lightNear)
*/

class Light : private ObjectTransformation {

public:
	float tmpXPitch, tmpYYaw;

	enum class Type {
		None = 0,
		Point = 1,
		Spot = 2
	};

	Light(Type type = Type::Point, bool shadowActive = false, unsigned int shadowResolution = 1024u);

	void setVisibility(bool visible);
	void setType(Type type);
	void setPosition(float x, float y, float z);
	void set(const glm::vec3& color, float intensity, float range);
	void setColor(const glm::vec3 &color);
	void setIntensity(float intensity);
	void setRange(float range);
	void setSpotAngles(float externalAngle, float internalAngle);
	void setDirection(float xPitchDeg, float yYawDeg);
	void setShadowPlanes(float near, float far);

	bool isVisible() const;
	Type getType() const;
	const glm::vec3& getPosition() const;
	const glm::mat3& getRotation() const;
	const glm::vec3 &getColor() const;
	float getIntensity() const;
	float getRange() const;
	float getSpotlightAngle() const;
	float getSpotlightInternalAngle() const;
	glm::vec3 getDirectionVector() const;
	bool isShadowActive() const;
	unsigned int getShadowMaxResolution() const;
	float getShadowNearPlane() const;
	float getShadowFarPlane() const;

private:
	bool visible;
	Type type;
	glm::vec3 color;
	float intensity;
	float range;
	float spotAngleDeg;
	float spotInternalAngleDeg;
	bool shadowActive;
	unsigned int shadowMaxRes;
	float shadowNearPlane;
	float shadowFarPlane;
};

} // namespace dj