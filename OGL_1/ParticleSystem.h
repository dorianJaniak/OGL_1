#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include <glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace dj
{
class TimeDrivenMovement;
} // namespace dj

namespace dj
{

class ParticleSystem : private NonCopyable
{
	const TimeDrivenMovement& tdm;
	unsigned int count;
	GLuint vao, vbo;
	GLuint instancesVBO;
	std::vector<glm::vec3> positions;
	std::vector<glm::mat4> transformations;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 boundary;
	glm::vec2 particleScale;
	float opacity;

public:
	ParticleSystem(const TimeDrivenMovement& tdm, unsigned int count) noexcept;
	~ParticleSystem();

	bool initBuffers();
	void setPosition(const glm::vec3& pos);
	void setBounds(float width, float depth, float height);
	void setVelocity(const glm::vec3& vel);
	void setParticleScale(const glm::vec2& scale);
	void setOpacity(float opacity);

	unsigned int getCount() const;
	const glm::vec2& getParticleScale() const;
	float getOpacity() const;

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