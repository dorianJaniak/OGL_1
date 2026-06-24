#include "MainWindow.h"
#include "Enums/LogCodes.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace dj;

MainWindow::MainWindow(std::shared_ptr<ILogger> logger, unsigned int width, unsigned int height, float aspectRatio)
	: NamedLoggingInstance(logger, "MainWindow")
	, window(nullptr)
	, width(width)
	, height(height)
	, aspectRatio(aspectRatio)
{

}

bool MainWindow::initGLFW(int majorGLVer, int minorGLVer, const char* name)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorGLVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorGLVer);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (!window)
	{
		log(LogLevel::Critical, LogCode::MainWindow_GLFW_Fail, "Could not create GLFW Window");
		terminate();
		return false;
	}

	initCallbacks();

	if (glewInit() != GLEW_OK)
	{
		log(LogLevel::Critical, LogCode::MainWindow_GLEW_Fail, "Could not init GLEW");
		terminate();
		return false;
	}

	return true;
}

void MainWindow::terminate()
{
	//glfwSetWindowUserPointer(window, nullptr);
	glfwTerminate();
	window = nullptr;
}

void MainWindow::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

int MainWindow::getWidth() const
{
	return width;
}

int MainWindow::getHeight() const
{
	return height;
}

float MainWindow::getAspectRatio() const
{
	return aspectRatio;
}

bool MainWindow::getShouldClose() const
{
	if (window)
	{
		return glfwWindowShouldClose(window);
	}

	return true;
}

void MainWindow::swapBuffers()
{
	if (window)
	{
		glfwSwapBuffers(window);
	}
}

void MainWindow::pollEvents()
{
	glfwPollEvents();
}

GLFWwindow* MainWindow::getWindow()
{
	return window;
}

MainWindow* MainWindow::getWindow(GLFWwindow* window)
{
	return static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
}

void MainWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	MainWindow* mw = getWindow(window);

	if (mw)
	{
		mw->propagateKeyCallback(key, scancode, action, mods);
	}
}

void MainWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
	MainWindow* mw = getWindow(window);

	if (mw)
	{
		unsigned int w = (width > 0 ? static_cast<unsigned int>(width) : 0u);
		unsigned int h = (height > 0 ? static_cast<unsigned int>(height) : 0u);
		mw->resizeFromExternal(w, h);
		mw->propagateResizeCallback(w, h);
	}
}

void MainWindow::cursorPosCallback(GLFWwindow* window, double xPix, double yPix)
{
	MainWindow* mw = getWindow(window);

	if (mw)
	{
		mw->propagateCursorPosCallback(
			xPix,
			yPix,
			xPix / static_cast<float>(mw->width),
			yPix / static_cast<float>(mw->height));
	}
}

void MainWindow::initCallbacks()
{
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, MainWindow::cursorPosCallback);
	glfwSetKeyCallback(window, MainWindow::keyCallback);
	glfwSetFramebufferSizeCallback(window, MainWindow::resizeCallback);
	//glfwSetScrollCallback(window, userScrollCallback);
}

void MainWindow::resizeFromExternal(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}