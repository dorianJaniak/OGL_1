#include "TGLTFLoader.h"
#include <iostream>
using namespace dj;

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

TGLTFLoader::TGLTFLoader()
{
	model = new tinygltf::Model();
	tgltf = new tinygltf::TinyGLTF();
}

TGLTFLoader::~TGLTFLoader()
{
	delete model;
	delete tgltf;
}

bool TGLTFLoader::load(const char* filename)
{
	std::string err;
	std::string warn;
	bool ret = tgltf->LoadASCIIFromFile(model, &err, &warn, filename);

	std::cout << "Bledy: " << err << "\nWarningi: " << warn << std::endl;

	return ret;
}