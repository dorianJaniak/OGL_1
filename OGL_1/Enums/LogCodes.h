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
};

} // namespace dj