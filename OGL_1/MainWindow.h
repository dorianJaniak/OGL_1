#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include "Logging/LoggingInstance.h"
#include "Logging/NamedLoggingInstance.h"
#include "InputSwitchboard.h"

struct GLFWwindow; 

namespace dj
{

class MainWindow : public NamedLoggingInstance, public InputSwitchboard, private NonCopyable, private NonMovable
{
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	float aspectRatio;

public:
	MainWindow(std::shared_ptr<ILogger> logger, unsigned int width, unsigned int height, float aspectRatio);

	bool initGLFW(int majorGLVer, int minorGLVer, const char* name);
	void terminate();

	void setSize(int width, int height);

	int getWidth() const;
	int getHeight() const;
	float getAspectRatio() const;
	bool getShouldClose() const;

	void swapBuffers();
	void pollEvents();

	GLFWwindow* getWindow();
	static MainWindow* getWindow(GLFWwindow* window);

protected:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPosCallback(GLFWwindow* window, double xPix, double yPix);

private:
	void initCallbacks();
	void resizeFromExternal(unsigned int width, unsigned int height);
};

} // namespace dj