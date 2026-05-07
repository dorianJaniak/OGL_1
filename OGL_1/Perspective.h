#pragma once
#include <glm.hpp>

namespace dj
{

class Perspective
{
	bool dirty;
	float fovDeg;
	float near;
	float far;
	float aspectRatio;
	glm::mat4 matrix;

public:
	Perspective() noexcept;
	Perspective(const Perspective&) noexcept = default;
	Perspective(Perspective&&) noexcept = default;

	void set(float fovDeg, float nearPlane, float farPlane, float aspectRatio);
	void setFOV(float fovDeg);
	void setPlanes(float nearPlane, float farPlane);
	void setAspectRatio(float ratio);

	const glm::mat4& getMatrix();

	float getFOV() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getAspectRatio() const;

private:
	void evaluate();
};

} // namespace dj