#include "Object.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>
using namespace dj;

Object::Object(const MeshAlignment& meshAlignment, const BoundingBox& boundingBox, std::string name)
	: meshAlignment(meshAlignment)
	, boundingBox(boundingBox)
	, defaultMaterial()
	, name(name)
{
}

Object::~Object()
{
	std::cout << "Object destructed: " << name << std::endl;
}

void Object::setDefaultMaterial(const MaterialWeakPtr mat)
{
	defaultMaterial = mat;
}

const BoundingBox& Object::getBoundingBox() const
{
	return boundingBox;
}

MaterialWeakPtr Object::getMaterial() const
{
	return defaultMaterial;
}

const MeshAlignment& Object::getMeshAlignment() const
{
	return meshAlignment;
}

const std::string& Object::getName() const
{
	return name;
}

void Object::setMeshAlignment(const MeshAlignment& meshIndices)
{
	meshAlignment = meshIndices;
}


ObjectInstance::ObjectInstance(ObjectPtrConst object, bool visible)
	: ObjectTransformation()
	, visible(visible)
	, material()
	, object(object)
{
}

ObjectInstance::~ObjectInstance()
{
	std::cout << "Instance destructed: " << object->getName() << std::endl;
}

void ObjectInstance::setVisible(bool visible)
{
	this->visible = visible;
}

void ObjectInstance::setMaterial(MaterialWeakPtr material)
{
	this->material = material;
}

bool ObjectInstance::isVisible() const
{
	return visible;
}

const BoundingBox& ObjectInstance::getBoundingBox()
{
	if (ObjectTransformation::isDirty())
	{
		ObjectTransformation::update();
		boundingBoxTransformed = object->getBoundingBox();
		for (unsigned int i = 0; i < 4u; ++i)
		{
			boundingBoxTransformed.front[i] = glm::vec3(getTransformation() * glm::vec4(boundingBoxTransformed.front[i], 1.0f));
			boundingBoxTransformed.back[i] = glm::vec3(getTransformation() * glm::vec4(boundingBoxTransformed.back[i], 1.0f));
		}
	}

	return boundingBoxTransformed;
}

ObjectPtrConst ObjectInstance::getObjectPtr() const
{
	return object;
}

MaterialWeakPtr ObjectInstance::getMaterial() const
{
	return material.expired() ? object->getMaterial() : material;
}

const MeshAlignment& ObjectInstance::getMeshAlignment() const
{
	return object->getMeshAlignment();
}
