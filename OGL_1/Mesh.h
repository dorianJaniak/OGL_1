#pragma once
#include "Definitions.h"
#include <vector>

namespace dj {

struct Vertex {
	float pos[3];
	float norm[3];
	float tan[3];
	float bitan[3];
	float uv[2];
};

class Mesh {
public:
	Mesh();
	void addVertices(const float vertices[], unsigned int vertsCount);
	void addIndices(const unsigned int indices[], unsigned int indicesCount);

	bool computeTangents();
	void computeBoundingBox();

	unsigned int getVerticesCount() const;
	unsigned int getIndicesCount() const;
	unsigned int getVerticesDataSize() const;
	unsigned int getIndicesDataSize() const;
	const Vertex* getVerticesPtr() const;
	const unsigned int* getIndicesPtr() const;
	const std::vector<Vertex> getVertices() const;
	const std::vector<unsigned int> getIndices() const;
	const BoundingBox& getBoundingBox() const;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	BoundingBox boundingBox;
};

class MeshMetadata
{
	unsigned int verticesCount;
	unsigned int indicesCount;
public:
	MeshMetadata(const Mesh& mesh);
	unsigned int getVerticesCount() const;
	unsigned int getIndicesCount() const;
};

} // namespace dj