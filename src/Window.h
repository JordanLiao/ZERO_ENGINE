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

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
};

#endif