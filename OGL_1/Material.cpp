#include "Material.h"
#include "Program.h"
using namespace dj;

Material::Material()
	: program()
{
}

void Material::setProgram(ProgramWeakPtr program)
{
	this->program = program;
}

bool Material::addTexture(const Handle& handle, const std::string& samplerName)
{
	bool updated = false;
	if (!program.expired())
	{
		unsigned int uniformLocation = program.lock()->getUniformLocation(samplerName);
		if (uniformLocation != -1)
		{
			updated = true;
			textures.push_back({handle, samplerName});

		/*	unsigned int index = getIndex(typeInfo);

			if (index < c_textureUnitsMax)
			{
				// Update texture binding for this unit
				textures[index].typeInfo = typeInfo;
			}
			else
			{
				// Add texture binding
				textures.push_back({ uniformIndex, typeInfo });
			}
			updated = true;*/
		}
	}

	return updated;
}

//void Material::bindTextures() const
//{
//	for (const TextureBinding& tex : textures)
//	{
//		glActiveTexture(GL_TEXTURE0 + tex.unit);
//		glBindTexture(tex.typeInfo.glType, tex.typeInfo.id);
//	}
//}

unsigned int Material::getTexturesCount() const
{
	return static_cast<unsigned int>(textures.size());
}

const std::vector<Material::TextureBinding>& Material::getTextures() const
{
	return textures;
}

//const std::vector<TextureID> Material::getTextures() const
//{
//	return textures;
//}

ProgramWeakPtr Material::getProgram() const
{
	return program;
}

unsigned int Material::getIndex(const Handle& handle) const
{
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		if (textures[i].handle == handle)
			return i;
	}

	return c_textureUnitsMax;
}