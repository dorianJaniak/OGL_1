#pragma once

struct GLFWwindow; 

namespace dj
{

class MainWindow
{
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	float aspectRatio;
	void (*userCursorPosCallback)(GLFWwindow*, double, double, double, double);

public:
	MainWindow(unsigned int width, unsigned int height, float aspectRatio);
	MainWindow(const MainWindow&) = delete;
	MainWindow(MainWindow&&) = delete;
	void operator=(const MainWindow&) = delete;

	bool initGLFW(int majorGLVer, int minorGLVer, const char* name);
	void terminate();

	void setSize(int width, int height);
	bool setKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods));
	bool setCursorPosCallback(void (*callback)(GLFWwindow* window, double xPix, double yPix, double xNorm, double yNorm));

	int getWidth() const;
	int getHeight() const;
	bool getShouldClose() const;

	void swapBuffers();
	void pollEvents();

	GLFWwindow* getWindow();
	static MainWindow* getWindow(GLFWwindow* window);

protected:
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPosCallback(GLFWwindow* window, double xPix, double yPix);

private:
	void resizeFromExternal(int width, int height);
};

} // namespace dj