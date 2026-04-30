#pragma once
#include <glm.hpp>

namespace dj 
{

class ObjectTransformation {
	mutable bool dirty;		// Mutabe due to lazy evaluation
	glm::vec3 translation;
	glm::vec3 scale;
	glm::mat4 rotation;
	mutable glm::mat4 model;	// Mutable due to lazy evaluation
	mutable glm::mat3 normalToModelSpace;	// Mutable due to lazy evaluation
public:
	ObjectTransformation();

	void update();
	void setTranslation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(const glm::mat4& rotation);

	const glm::mat4& getTransformation() const;
	const glm::mat3& getNormalToModelSpace() const;
	const glm::vec3& getTranslation() const;
	const glm::vec3& getScale() const;
	const glm::mat4& getRotation() const;

protected:
	bool isDirty() const;

private:
	// Update that is executed for const objects - modifies mutable members
	// lazy evaluation
	void evaluate() const;
};

} // namespace dj