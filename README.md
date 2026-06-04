# Disclaimer: The Ugly Part

The engine is currently in an early stage of intensive development and significant architectural refactoring. Initially, the project was created primarily as a way to familiarize myself with OpenGL and was never intended for public release. Over time, I decided to continue developing it as a long-term learning project and a platform for experimenting with new ideas.

At present, the core architecture is undergoing major redesigns, and most of the systems are still subject to change. The project serves as a personal sandbox where I can experiment with the OpenGL graphics pipeline, deepen my knowledge of modern C++, and explore software architecture and performance-oriented design.

The goal is not to build an engine that competes with established commercial solutions. Instead, this is a purely educational project focused on learning, experimentation, and continuous improvement of my software engineering skills.

## If you look for the ugliest part

The ugliest part of the project is main source file: [OGL_1.cpp](OGL_1/OGL_1.cpp). It is pure evil.

## If you look for the nicest part

The nicest part of the project is recently redesigned Texture management system, which includes classes:
* [TextureManager](OGL_1/TextureManager.h),
* [TextureResource](OGL_1/TextureResource.h),
* [TextureDescriptor](OGL_1/Descriptors/TextureDesc.h),
* [Handle](OGL_1/Handle.h)

Texture Management is a good example of the engine's future direction. It ensures good memory locality (CPU's cache locality).

Also worth noting:
* [IRenderNode](OGL_1/RenderNodes/IRenderNode.h) class,
* [QualitySettings](OGL_1/QualitySettings.h) class,
* [Program](OGL_1/Program.h) class,
* all headers in [Enums](OGL_1/Enums/).

# Goals

As said above, main purpose of implementing this (yet another) graphic engine is self-improvement in: OpenGL, modern C++, efficient code writing.

The following functionality is planned to be implemented in the future:
* loading the scene from GLTF file,
* bindless textures,
* uniform arrays,
* error logging system,
* LOD system,
* simple objects movement (without using bone system),
* resource management (framebuffers, textures, objects, object instances, materials, etc.),
* rendering graph based on nodes and passes,
* debug rendering layer,
* alpha blending and transparency,
* cascaded shadows,
* runtime shader generation,
* pre-rasterization object culling (avoiding draw calls for objects behind the camera),
* depth of field effect,
* bloom effect (and other simple fullscreen effects),
* reflection probes,
* HDR,
* volumetric effects.

# Third party:

Needs to be localised in: ../third_party/
- glew-2.1.0-win32
- glfw-3.4.bin.WIN64
- glm-master
- stb-master
- tinygltf-release