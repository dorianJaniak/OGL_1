#include "IRenderWorldNode.h"
#include "../Material.h"
#include "../Program.h"
using namespace dj;

IRenderWorldNode::IRenderWorldNode(const TextureManager& texMgr, FramebufferPtr output, GLuint ebo, std::vector<dj::ObjectInstancePtr>& objectInstances, const std::string& name)
	: IRenderNode(texMgr, output, name)
	, objectInstances(objectInstances)
	, ebo(ebo)
	, customMaterial(nullptr)
{
}

void IRenderWorldNode::setCustomMaterial(MaterialPtr material)
{
	customMaterial = material;
}

void IRenderWorldNode::draw()
{
	static const unsigned int c_texUnitPostProgramLevel = 0u;
	// sortObjects();

	uboPerFrame();
	std::shared_ptr<const dj::Material> previousMaterial = nullptr;
	std::shared_ptr<const dj::Material> activeMaterial = nullptr;

	std::shared_ptr<const Program> previousProgram = nullptr;
	std::shared_ptr<const Program> activeProgram = nullptr;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	for (dj::ObjectInstancePtr obj : objectInstances)
	{
		if (obj->isVisible())
		{
			if (!obj->getMaterial().expired())
			{
				activeMaterial = obj->getMaterial().lock();
				if (customMaterial != nullptr)
				{
					activeMaterial = customMaterial;
				}
				
				if (activeMaterial != previousMaterial)
				{
					beginBindingTextures();
					if (!activeMaterial->getProgram().expired())
					{
						activeProgram = activeMaterial->getProgram().lock();
						if (activeProgram != previousProgram)
						{
							activeProgram->use();
							uniformPerProgram(activeProgram);
							bindAndUniformNodeTextures(activeProgram);
							previousProgram = activeProgram;

							saveTextureUnitNo(c_texUnitPostProgramLevel);
						}
					}

					restoreTextureUnitNo(c_texUnitPostProgramLevel);

					bindMaterialTextures(activeProgram, activeMaterial);
					bindAndUniformTextures();
					uniformPerMaterial(activeProgram, activeMaterial);
					previousMaterial = activeMaterial;
				}
			}

			uniformPerObject(activeProgram, obj);
			glDrawRangeElements(
				GL_TRIANGLES,
				obj->getMeshAlignment().verticesStart,
				obj->getMeshAlignment().verticesEnd,
				obj->getMeshAlignment().indicesCount, GL_UNSIGNED_INT,
				(void*)(obj->getMeshAlignment().indicesStart * sizeof(unsigned int)));
		}
	}
}

void IRenderWorldNode::bindMaterialTextures(ConstProgramPtr program, ConstMaterialPtr material)
{
	for (const Material::TextureBinding& tex : material->getTextures())
	{
		bindAndUniformTexture(tex.handle, program->getUniformLocation(tex.uniformName));

		//if (!getFreeTextureUnitsCount())
		//{
		//	// TODO: In future glUniform1i should not be called here, it should be called once per program change (not material)
		//	glUniform1i(program->getUniformLocation(tex.uniformName), getTextureUnitNo());
		//	bindTexture(tex.typeInfo);
		//}
	}
}

void IRenderWorldNode::bindAndUniformTextures()
{
}