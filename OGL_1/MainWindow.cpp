#include "MainWindow.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace dj;

MainWindow::MainWindow(unsigned int width, unsigned int height, float aspectRatio)
	: window(nullptr)
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
		std::cerr << "Could not create GLFW Window\n";
		terminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, MainWindow::resizeCallback);
	//glfwSetScrollCallback(window, userScrollCallback);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Could not init GLEW\n";
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

bool MainWindow::setKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
{
	if (window)
	{
		glfwSetKeyCallback(window, callback);
		return true;
	}

	return false;
}

bool MainWindow::setCursorPosCallback(void (*callback)(GLFWwindow* window, double xPix, double yPix, double xNorm, double yNorm))
{
	if (window)
	{
		userCursorPosCallback = callback;
		glfwSetCursorPosCallback(window, cursorPosCallback);

		return false;
	}

	return true;
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

void MainWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
	MainWindow* mw = getWindow(window);

	if (mw)
	{
		mw->resizeFromExternal(width, height);
	}
}

void MainWindow::cursorPosCallback(GLFWwindow* window, double xPix, double yPix)
{
	MainWindow* mw = getWindow(window);

	if (mw)
	{
		mw->userCursorPosCallback(window,
			xPix,
			yPix,
			xPix / static_cast<float>(mw->width),
			yPix / static_cast<float>(mw->height));
	}
}

void MainWindow::resizeFromExternal(int width, int height)
{
	this->width = width;
	this->height = height;
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}