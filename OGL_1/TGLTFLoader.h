#pragma once

namespace tinygltf
{
	class Model;
	class TinyGLTF;
}

namespace dj
{
class TGLTFLoader
{
	tinygltf::Model* model;
	tinygltf::TinyGLTF* tgltf;
public:
	TGLTFLoader();
	~TGLTFLoader();

	bool load(const char *filename);
};

} // namespace dj