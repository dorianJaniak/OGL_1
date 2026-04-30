#include "ObjectTransformation.h"
#include <gtc/matrix_transform.hpp>
using namespace dj;

ObjectTransformation::ObjectTransformation()
	: dirty(true)
	, translation(glm::vec3(0.0f, 0.0f, 0.0f))
	, scale(glm::vec3(1.0f, 1.0f, 1.0f))
	, rotation(glm::mat4(1.0f))
	, model(glm::mat4(1.0f))
	, normalToModelSpace(glm::mat4(1.0f))
{ }

void ObjectTransformation::update()
{
	evaluate();
}

void ObjectTransformation::setTranslation(float x, float y, float z)
{
	translation.x = x;
	translation.y = y;
	translation.z = z;
	dirty = true;
}

void ObjectTransformation::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	dirty = true;
}

void ObjectTransformation::setRotation(const glm::mat4& rotation)
{
	this->rotation = rotation;
	dirty = true;
}

// In World Space (where Z is to camera)
const glm::mat4& ObjectTransformation::getTransformation() const
{
	evaluate();
	return model;
}

const glm::mat3& ObjectTransformation::getNormalToModelSpace() const
{
	evaluate();
	return normalToModelSpace;
}

const glm::vec3& ObjectTransformation::getTranslation() const 
{
	return translation;
}

const glm::vec3& ObjectTransformation::getScale() const
{
	return scale;
}

const glm::mat4& ObjectTransformation::getRotation() const
{
	return rotation;
}

bool ObjectTransformation::isDirty() const
{
	return dirty;
}

void ObjectTransformation::evaluate() const
{
	if (dirty)
	{
		// In glm glm::scale(A, b) means that result will be like A * Mat4(b)
		// Which means that below equals to:
		// (mat4(1.0f) * Mat4(tran) * rotation) * Mat4(scale) 
		//model = glm::translate(glm::mat4(1.0f), translation) * rotation;
		//model = glm::scale(model, scale);
		glm::mat4 tran = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 scal = glm::scale(glm::mat4(1.0f), scale);
		model = tran * rotation * scal;
		normalToModelSpace = glm::mat3(glm::transpose(glm::inverse(model)));
		dirty = false;
	}
}