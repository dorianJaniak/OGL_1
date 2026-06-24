#include "Mesh.h"
#include "Logging/Log.h"
#include "Enums/LogCodes.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace dj;

Mesh::Mesh()
{
}

void Mesh::addVertices(const float vertices[], unsigned int vertsCount)
{
	for (unsigned int i = 0; i < vertsCount; ++i)
	{
		Vertex v;
		v.pos[0] = vertices[i * 8];
		v.pos[1] = vertices[i * 8 + 1];
		v.pos[2] = vertices[i * 8 + 2];

		v.norm[0] = vertices[i * 8 + 3];
		v.norm[1] = vertices[i * 8 + 4];
		v.norm[2] = vertices[i * 8 + 5];

		v.uv[0] = vertices[i * 8 + 6];
		v.uv[1] = vertices[i * 8 + 7];

		this->vertices.push_back(v);
	}
}

void Mesh::addIndices(const unsigned int indices[], unsigned int indicesCount)
{
	for (unsigned int i = 0; i < indicesCount; ++i)
	{
		this->indices.push_back(indices[i]);
	}
}

bool Mesh::computeTangents(std::shared_ptr<ILogger> logger)
{
	static unsigned meshCount = 0;
	if ((indices.size() % 3) || indices.size() == 0)
	{
		return false;
	}

	for (unsigned int i = 0; i < (indices.size() / 3u); ++i)
	{
		unsigned int index1 = indices[i * 3u];
		unsigned int index2 = indices[i * 3u + 1];
		unsigned int index3 = indices[i * 3u + 2];
		
		if (index1 >= vertices.size() || index2 >= vertices.size() || index3 >= vertices.size())
		{
			return false;
		}

		Vertex& v1 = vertices[index1];
		Vertex& v2 = vertices[index2];
		Vertex& v3 = vertices[index3];

		glm::vec3 pos1 = glm::vec3(v1.pos[0], v1.pos[1], v1.pos[2]);
		glm::vec3 pos2 = glm::vec3(v2.pos[0], v2.pos[1], v2.pos[2]);
		glm::vec3 pos3 = glm::vec3(v3.pos[0], v3.pos[1], v3.pos[2]);
		glm::vec2 uv1 = glm::vec2(v1.uv[0], v1.uv[1]);
		glm::vec2 uv2 = glm::vec2(v2.uv[0], v2.uv[1]);
		glm::vec2 uv3 = glm::vec2(v3.uv[0], v3.uv[1]);

		glm::vec3 e1 = pos2 - pos1;
		glm::vec3 e2 = pos3 - pos1;

		glm::vec2 duv1 = uv2 - uv1;
		glm::vec2 duv2 = uv3 - uv1;

		if (logger && glm::cross(glm::vec3(duv1.x, duv1.y, 0.0f), glm::vec3(duv2.x, duv2.y, 0.0f)).z < 0.0f)
		{
			logger->log(Log(LogLevel::Warning, LogCode::Mesh_UVOrderComputation_Fail, "", "Wrong order of UV for Mesh {} point: ({}, {}, {}) and ({}, {}, {}) and ({}, {}, {})",
				meshCount,
				pos1.x, pos1.y, pos1.z,
				pos2.x, pos2.y, pos2.z,
				pos3.x, pos3.y, pos3.z));
		}


		float f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);

		glm::vec3 tangent = f * (duv2.y * e1 - duv1.y * e2);
		glm::vec3 bitangent = f * (-duv2.x * e1 + duv1.x * e2);

		// When issues with symmetric meshes will appear
		//glm::vec3 n1(v1.norm[0], v1.norm[1], v1.norm[2]);
		//if (glm::dot(glm::cross(n1, tangent), bitangent) < 0.0f)
		//{
		//	tangent = -tangent;
		//}

		v1.tan[0] = v2.tan[0] = v3.tan[0] = tangent.x;
		v1.tan[1] = v2.tan[1] = v3.tan[1] = tangent.y;
		v1.tan[2] = v2.tan[2] = v3.tan[2] = tangent.z;

		v1.bitan[0] = v2.bitan[0] = v3.bitan[0] = bitangent.x;
		v1.bitan[1] = v2.bitan[1] = v3.bitan[1] = bitangent.y;
		v1.bitan[2] = v2.bitan[2] = v3.bitan[2] = bitangent.z;
	}

	meshCount++;

	return true;
}

void Mesh::computeBoundingBox()
{
	float xPos = 0.0f;
	float xNeg = 0.0f;
	float yPos = 0.0f;
	float yNeg = 0.0f;
	float zPos = 0.0f;
	float zNeg = 0.0f;
	if (vertices.size())
	{
		xPos = vertices[0].pos[0];
		yPos = vertices[0].pos[1];
		zPos = vertices[0].pos[2];

		xNeg = xPos;
		yNeg = yPos;
		zNeg = zPos;
	}


	for (const auto& v : vertices)
	{
		xPos = (v.pos[0] > xPos ? v.pos[0] : xPos);
		yPos = (v.pos[1] > yPos ? v.pos[1] : yPos);
		zPos = (v.pos[2] > zPos ? v.pos[2] : zPos);
		
		xNeg = (v.pos[0] < xNeg ? v.pos[0] : xNeg);
		yNeg = (v.pos[1] < yNeg ? v.pos[1] : yNeg);
		zNeg = (v.pos[2] < zNeg ? v.pos[2] : zNeg);
	}

	boundingBox = BoundingBox(xPos, xNeg, yPos, yNeg, zPos, zNeg);
}

unsigned int Mesh::getVerticesCount() const
{
	return static_cast<unsigned int>(vertices.size());
}

unsigned int Mesh::getIndicesCount() const
{
	return static_cast<unsigned int>(indices.size());
}

unsigned int Mesh::getVerticesDataSize() const
{
	return static_cast<unsigned int>(sizeof(Vertex) * vertices.size());
}

unsigned int Mesh::getIndicesDataSize() const
{
	return static_cast<unsigned int>(sizeof(unsigned int) * indices.size());
}

const Vertex* Mesh::getVerticesPtr() const
{
	return &vertices[0];
}

const unsigned int* Mesh::getIndicesPtr() const
{
	return &indices[0];
}

const std::vector<Vertex> Mesh::getVertices() const
{
	return vertices;
}

const std::vector<unsigned int> Mesh::getIndices() const
{
	return indices;
}

const BoundingBox& Mesh::getBoundingBox() const
{
	return boundingBox;
}

MeshMetadata::MeshMetadata(const Mesh& mesh)
	: verticesCount(mesh.getVerticesCount())
	, indicesCount(mesh.getIndicesCount())
{
}
unsigned int MeshMetadata::getVerticesCount() const
{
	return verticesCount;
}

unsigned int MeshMetadata::getIndicesCount() const
{
	return indicesCount;
}