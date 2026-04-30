#include "MeshData.h"
#include <algorithm>
using namespace dj;


MeshData::MeshData()
	: maxIndex(0)
{
}

MeshAlignment MeshData::addMesh(const Mesh& mesh)
{
	MeshAlignment mi;
	//meshes.push_back(mesh);
	const std::vector<Vertex>& meshVertices = mesh.getVertices();
	const std::vector<Index> &meshIndices = mesh.getIndices();

	// First index starts at the end of current indices vector
	mi.indicesStart = static_cast<Index>(indices.size());
	mi.verticesStart = static_cast<Index>(vertices.size());

	Index maxIndexInMesh = 0;
	for (Index i : meshIndices)
	{
		// Looking for maximum value of Index
		maxIndexInMesh = (i > maxIndexInMesh) ? i : maxIndexInMesh;

		// Shifting value of the Index to recognize previous Indices
		// Adding to indices vector
		indices.push_back(i + mi.verticesStart);
	}

	for (const Vertex& v : meshVertices)
	{
		vertices.push_back(v);
	}

	mi.verticesEnd = static_cast<unsigned int>(vertices.size()) - 1u;
	mi.verticesCount = static_cast<unsigned int>(meshVertices.size());
	mi.indicesCount = static_cast<unsigned int>(meshIndices.size());

	return mi;
}

unsigned int MeshData::getVerticesCount() const
{
	//unsigned int count = 0;

	//for (const Mesh& m : meshes)
	//{
	//	count += m.getVerticesCount();
	//}

	//return count;
	return static_cast<unsigned int>(vertices.size());
}

unsigned int MeshData::getIndicesCount() const
{
	//unsigned int count = 0;

	//for (const Mesh& m : meshes)
	//{
	//	count += m.getIndicesCount();
	//}

	//return count;

	return static_cast<unsigned int>(indices.size());
}

unsigned int MeshData::getVerticesDataSize() const
{
	return (sizeof(Vertex) * getVerticesCount());
}

unsigned int MeshData::getIndicesDataSize() const
{
	return (sizeof(Index) * getIndicesCount());
}

//IndicesFromAndCount MeshData::getVerticesFromAndCount() const
//{
//	return getVerticesFromAndCount(static_cast<unsigned int>(meshes.size() - 1u));
//}
//
//IndicesFromAndCount MeshData::getIndicesFromAndCount() const
//{
//	return getIndicesFromAndCount(static_cast<unsigned int>(meshes.size() - 1u));
//}

const std::vector<Vertex> &MeshData::getAllVertices() const
{
	//std::vector<Vertex> vertices;
	//for(const Mesh & m : meshes)
	//{
	//	const std::vector<Vertex> meshVertices = m.getVertices();

	//	vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
	//}

	//return vertices;

	return vertices;
}

const std::vector<Index> &MeshData::getAllIndices() const
{
	//std::vector<unsigned int> indices;
	//for (const Mesh& m : meshes)
	//{
	//	const std::vector<unsigned int> meshIndices = m.getIndices();

	//	indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());
	//}

	//return indices;

	return indices;
}

//IndicesFromAndCount MeshData::getVerticesFromAndCount(unsigned int meshIndex) const
//{
//	unsigned int startIndex = 0;
//	unsigned int count = 0;
//
//	if (meshIndex < meshes.size())
//	{
//		for (unsigned int i = 0; i < meshIndex; ++i)
//		{
//			startIndex += meshes[i].getVerticesCount();
//		}
//
//		count = meshes[meshIndex].getVerticesCount();
//	}
//
//	return IndicesFromAndCount(startIndex, count);
//}
//
//IndicesFromAndCount MeshData::getIndicesFromAndCount(unsigned int meshIndex) const
//{
//	unsigned int startIndex = 0;
//	unsigned int count = 0;
//
//	if (meshIndex < meshes.size())
//	{
//		for (unsigned int i = 0; i < meshIndex; ++i)
//		{
//			startIndex += meshes[i].getIndicesCount();
//		}
//
//		count = meshes[meshIndex].getIndicesCount();
//	}
//
//	return IndicesFromAndCount(startIndex, count);
//}