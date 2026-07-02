#include "ParticleSystem.h"
#include "Time/TimeDrivenMovement.h"
#include "Predefinitions/PredefinedMeshes.h"
#include <gtc/matrix_transform.hpp>
#include <random>
using namespace dj;

ParticleSystem::ParticleSystem(const TimeDrivenMovement& tdm) noexcept
	: tdm(tdm)
	, vao(0)
	, vbo(0)
	, instancesVBO(0)
	, position{}
	, velocity{}
	, boundary{ 1.0f, 1.0f, 1.0f }
{
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &instancesVBO);
	glDeleteVertexArrays(1, &vao);
}

void ParticleSystem::initBuffers()
{
	if (vao || vbo || instancesVBO)
	{
		return;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Store particle's mesh
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particlePlaneVerts), particlePlaneVerts, GL_STATIC_DRAW);
	
	// Configure attributes - mesh
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)NULL);				// position
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	// UV
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 0);

	// Prepare memory for particle instances
	glGenBuffers(1, &instancesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancesVBO);
	glBufferData(GL_ARRAY_BUFFER, count() * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	// Configure attributes - instancing
	for (unsigned int i = 0; i < 4; ++i)
	{
		GLuint attribLoc = 2 + i;
		glEnableVertexAttribArray(attribLoc);
		glVertexAttribPointer(attribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
		glVertexAttribDivisor(attribLoc, 1);		// 1 - by instance
	}

	glBindVertexArray(0);
}

void ParticleSystem::setPosition(const glm::vec3& pos)
{
	position = pos;
}

void ParticleSystem::setBounds(float width, float depth, float height)
{
	boundary = glm::vec3(width, depth, height);
}

void ParticleSystem::setVelocity(const glm::vec3& vel)
{
	velocity = vel;
}

unsigned int ParticleSystem::count() const
{
	return static_cast<unsigned int>(std::min(positions.size(), transformations.size()));
}

void ParticleSystem::randomizeParticles()
{
	glm::vec3 halfBoundary = boundary * 0.5f;

	for (glm::vec3& pos : positions)
	{
		pos.x = genRandomValue(-halfBoundary.x, halfBoundary.x);
		pos.y = genRandomValue(-halfBoundary.y, halfBoundary.y);
		pos.z = genRandomValue(-halfBoundary.z, halfBoundary.z);
	}
}

void ParticleSystem::updateTransformations()
{
	unsigned int c = count();
	for (unsigned int i = 0; i < c; ++i)
	{
		transformations[i] = glm::translate(glm::mat4(1.0f), (position + positions[i]));
	}

	glBindBuffer(GL_ARRAY_BUFFER, instancesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, c * sizeof(glm::mat4), transformations.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::bind()
{
	glBindVertexArray(vao);
}

void ParticleSystem::unbind()
{
	glBindVertexArray(0);
}

void ParticleSystem::updatePositions()
{
	static bool first = true;
	if (first)
	{
		first = false;
		return;
	}
	float step = static_cast<float>(tdm.getFrameDiffMs().count()) / 1000.0f;
	glm::vec3 velStep = velocity * step;

	glm::vec3 maxBoundary = boundary * 0.5f;
	glm::vec3 minBoundary = -maxBoundary;

	for (glm::vec3& pos : positions)
	{
		pos += velStep;
		if (pos.x < minBoundary.x || pos.x > maxBoundary.x
			|| pos.y < minBoundary.y || pos.y > maxBoundary.y
			|| pos.z < minBoundary.z || pos.z > maxBoundary.z)
		{
			randomizeStartingPosition(pos, maxBoundary.x, maxBoundary.y, maxBoundary.z);
		}
	}
}

void ParticleSystem::randomizeStartingPosition(glm::vec3& pos, float halfX, float halfY, float halfZ)
{
	pos.x = genRandomValue(-halfX, halfX);
	pos.y = genRandomValue(halfY * 0.95f, halfY);
	pos.z = genRandomValue(-halfZ, halfZ);
}

float ParticleSystem::genRandomValue(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 genBits(rd());

	std::uniform_real_distribution<float> dis(min, max);

	return dis(genBits);
}