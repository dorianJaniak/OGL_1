#pragma once
#include "ObjectTransformation.h"
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include "Definitions.h"
#include <memory>
#include <string>

namespace dj {
	class Program;
	class Material;
	//class MeshPacker;	// TODO: Class that packs meshes in GPU memory
}

namespace dj {

	class Object
	{
		// Mesh Packer might need to re-align meshes in GPU memory for better performance
		//friend dj::MeshPacker;

		MeshAlignment meshAlignment;
		BoundingBox boundingBox;
		MaterialWeakPtr defaultMaterial;
		std::string name;
	public:
		Object(const MeshAlignment& meshIndices, const BoundingBox& boundingBox, std::string name = "");
		~Object();

		void setDefaultMaterial(const MaterialWeakPtr mat);
		
		const BoundingBox& getBoundingBox() const;
		MaterialWeakPtr getMaterial() const;
		const MeshAlignment& getMeshAlignment() const;
		const std::string& getName() const;

	private:
		void setMeshAlignment(const MeshAlignment& meshIndices);
	};

	class ObjectInstance : public ObjectTransformation
	{
		bool visible;
		BoundingBox boundingBoxTransformed;
		MaterialWeakPtr material;
		ObjectPtrConst object;

	public:
		ObjectInstance(ObjectPtrConst object, bool visible = true);
		ObjectInstance(const ObjectInstance&) = default;
		~ObjectInstance();

		void setVisible(bool visible);
		void setMaterial(const MaterialWeakPtr mat);

		bool isVisible() const;
		const BoundingBox& getBoundingBox();
		ObjectPtrConst getObjectPtr() const;
		MaterialWeakPtr getMaterial() const;
		const MeshAlignment& getMeshAlignment() const;
	};



} // namespace dj