#pragma once

#include "Mesh.h"
#include "Definitions.h"

#include <vector>

namespace dj {

	class MeshData {
		//std::vector<Mesh> meshes;
		std::vector<Vertex> vertices;
		std::vector<Index> indices;

		unsigned int maxIndex;
	public:

		static const unsigned int vertexComponentsCount = 3 + 3 + 3 + 3 + 2;

		MeshData();
		MeshData(const MeshData&) = delete;
		MeshData(MeshData&&) = delete;

		MeshAlignment addMesh(const Mesh& mesh);

		unsigned int getVerticesCount() const;
		unsigned int getIndicesCount() const;
		unsigned int getVerticesDataSize() const;
		unsigned int getIndicesDataSize() const;
		//IndicesFromAndCount getVerticesFromAndCount() const;
		//IndicesFromAndCount getIndicesFromAndCount() const;
		const std::vector<Vertex> &getAllVertices() const;
		const std::vector<Index> &getAllIndices() const;

	private:
		//IndicesFromAndCount getVerticesFromAndCount(unsigned int meshIndex) const;
		//IndicesFromAndCount getIndicesFromAndCount(unsigned int meshIndex) const;
	};

} // namespace dj