#pragma once

namespace dj
{

enum LogCode
{
	Unknown,
	Object_TangentComputation_Fail,
	MainWindow_GLFW_Fail,
	MainWindow_GLEW_Fail,
	Shader_Compilation_Fail,
	Program_Initialized_Fail,
	Program_Link_Fail,
	TexMgr_TextureLoading_Fail,
	TexMgr_TextureResolution_Fail,
	TexMgr_TextureColorFormat_Fail,
	TexMgr_CubemapNotSquare,
	TexMgr_ResolutionMismatch,
	TexMgr_ColorFormatMismatch,
	FboMgr_TextureCreation_Fail,
	FboMgr_TextureAttachment_Fail,
	FboMgr_RenderBufferAttachment_Fail,
	FboMgr_FramebufferStatus_Fail,
	FboMgr_FramebufferNotAdded_Fail,
};

} // namespace dj