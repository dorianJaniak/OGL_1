#pragma once
#include <glm.hpp>
#include <utility>
#include <iostream>
#include <memory>
#include <array>

namespace dj {
	enum class EngineProgramID
	{
		prePBR = 0u,
		postProcessing = 1u,
		depth = 2u,
		skybox = 3u,
		debugMesh = 4u,
		depthCube = 5u,
	};

	class Material;
	class Program;
	class Object;
	class ObjectInstance;
	class ObjectTransformation;
	class Light;
	class Texture;
	class Framebuffer;
	class Camera;

	using IndicesFromAndCount = std::pair<unsigned int, unsigned int>;
	using Index = unsigned int;
	using MaterialPtr = std::shared_ptr<Material>;
	using MaterialWeakPtr = std::weak_ptr<const Material>;
	using ConstMaterialPtr = std::shared_ptr<const Material>;
	using ProgramPtr = std::shared_ptr<Program>;
	using ProgramPtrConst = std::shared_ptr<const Program>;
	using ConstProgramPtr = std::shared_ptr<const Program>;
	using ProgramWeakPtr = std::weak_ptr<const Program>;
	using ObjectPtr = std::shared_ptr<Object>;
	using ObjectPtrConst = std::shared_ptr<const Object>;
	using ObjectConstPtrConst = const std::shared_ptr<const Object>;
	using ObjectWeakPtr = std::weak_ptr<const Object>;
	using ObjectInstancePtr = std::shared_ptr<ObjectInstance>;
	using ObjectTransformationPtr = std::shared_ptr<ObjectTransformation>;
	using ObjectTransformationWeakPtr = std::weak_ptr<ObjectTransformation>;
	using LightPtr = std::shared_ptr<Light>;
	using LightWeakPtr = std::weak_ptr<Light>;
	using CameraPtr = std::shared_ptr<Camera>;

	using ColorRGBA = std::array<float, 4u>;

	struct MeshAlignment {
		Index indicesStart;
		unsigned int verticesCount;
		unsigned int indicesCount;
		Index verticesStart;
		Index verticesEnd;
	};

	//struct BoundingBox
	//{
	//	glm::vec3 pos;
	//	glm::vec3 neg;
	//};

	struct BoundingBox
	{
		glm::vec3 front[4];
		glm::vec3 back[4];

		BoundingBox(float xPos = 0.0f, float xNeg = 0.0f,
			float yPos = 0.0f, float yNeg = 0.0f,
			float zPos = 0.0f, float zNeg = 0.0f)
		{
			front[0] = glm::vec3(xNeg, yPos, zPos);
			front[1] = glm::vec3(xNeg, yPos, zNeg);
			front[2] = glm::vec3(xPos, yPos, zNeg);
			front[3] = glm::vec3(xPos, yPos, zPos);

			back[0] = glm::vec3(xNeg, yNeg, zPos);
			back[1] = glm::vec3(xNeg, yNeg, zNeg);
			back[2] = glm::vec3(xPos, yNeg, zNeg);
			back[3] = glm::vec3(xPos, yNeg, zPos);
		}
	};

	struct BoundingPlane
	{
		glm::vec2 pos;
		glm::vec2 neg;
	};

	struct Log {
		static constexpr unsigned cMaxLength = 1024;
		char log[cMaxLength];
		std::string name;
		int ok;

		Log()
		{
			log[0] = '\0';
		}

		void setName(const std::string& name)
		{
			this->name = name;
		}

		static const char* failPrefix()
		{
			return ">>FAIL: ";
		}

		static const char* warnPrefix()
		{
			return ">Warn: ";
		}

		static const char* okPrefix()
		{
			return "OK: ";
		}

		static const char* infoPrefix()
		{
			return "INFO: ";
		}

		void print() const
		{
			print(name.c_str());
		}

		void print(const char* procedureDescription) const
		{
			if (ok)
			{
				std::cout << okPrefix() << procedureDescription << std::endl;
			}
			else
			{
				std::cerr << failPrefix() << procedureDescription << std::endl;
				std::cerr << log << std::endl << std::endl;
			}
		}
	};
}