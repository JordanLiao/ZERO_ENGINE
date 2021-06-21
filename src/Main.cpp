#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>

#include "Window.h"
#include "Core.h"

#include "Renderer/ResourceLoaders.h"
#include <unordered_map>

void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);

	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << " \n"<<std::endl;
}

void beginTests() {
	std::unordered_map<std::string, Resources::Material*> * map = ResourceLoaders::loadMaterialMap("Assets/lowpolypine.mtl");
	if(map != NULL)
		std::cout << map->size() << std::endl;
}

int main(int argc, char* argv[])
{
	/*GLFWwindow* window = Window::createWindow(800, 600);	
	if (!window) exit(EXIT_FAILURE);

	print_versions();
	setup_callbacks(window);
	setup_opengl_settings();

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);

	while (!glfwWindowShouldClose(window))
	{
		//updating game state here
		Window::idleCallback();	
		Window::displayCallback(window);
	}

	Window::cleanUp();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);*/
	
	beginTests();
}

