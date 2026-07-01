#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include <glm.hpp>
#include <GL/glew.h>
#include <array>

namespace dj
{
class TimeDrivenMovement;
} // namespace dj

namespace dj
{

class ParticleSystem : private NonCopyable
{
	const TimeDrivenMovement& tdm;
	GLuint vao, vbo;
	GLuint instancesVBO;
	std::array<glm::vec3, 5000> positions;
	std::array<glm::mat4, 5000> transformations;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 boundary;

public:
	ParticleSystem(const TimeDrivenMovement& tdm) noexcept;
	~ParticleSystem();

	void initBuffers();
	void setPosition(const glm::vec3& pos);
	void setBounds(float width, float depth, float height);
	void setVelocity(const glm::vec3& vel);

	unsigned int count() const;

	void randomizeParticles();

	void updateTransformations();
	void bind();
	void unbind();

	void updatePositions();

private:
	void randomizeStartingPosition(glm::vec3& pos, float halfX, float halfY, float halfZ);
	float genRandomValue(float min, float max);
};

} // namespace dj