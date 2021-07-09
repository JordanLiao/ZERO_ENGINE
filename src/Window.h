#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Renderer/Shader.h"
#include "Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Object.h"
#include "Renderer/ResourceManager.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Renderer tools
	static Shader* shader;
	static Renderer renderer;
	static ResourceManager resourceManager;

	static Object* obj;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY); 
};

#endif