#ifndef _ENGINEAPP_H_
#define _ENGINEAPP_H_

#include "Renderer/Renderer.h"
#include "UIManager.h"

class EngineApp {
private:
	static Renderer renderer;
	static GLFWwindow* window;
	static UIManager ui;

public:
	//application callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback();

	static bool initializeProgram(GLFWwindow * w);
	static bool initializeObjects();
	static void cleanUp();
};

#endif