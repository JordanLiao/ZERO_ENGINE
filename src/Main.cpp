#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>

#include "Window.h"
#include "EngineApp.h"
#include "UIManager.h"
#include "Core.h"

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
	glfwSetKeyCallback(window, EngineApp::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, EngineApp::mouse_callback);
	glfwSetScrollCallback(window, EngineApp::scroll_callback);
	glfwSetCursorPosCallback(window, EngineApp::cursor_callback);
}

void setup_opengl_settings(){
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << " \n"<<std::endl;
}

void beginTests() {

}

int main(int argc, char* argv[])
{
	int initWidth = 1920;
	int initHeight = 1001;
	
	GLFWwindow* window = Window::createWindow(initWidth, initHeight);	
	if (!window) exit(EXIT_FAILURE);

	print_versions();
	setup_callbacks(window);
	setup_opengl_settings();

	//initialize the renderer & ui manager
	Renderer::initRenderer(NULL); // use default camera
	// Initialize the shader program; exit if initialization fails.
	if (!EngineApp::initializeProgram(window)) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!EngineApp::initializeObjects()) exit(EXIT_FAILURE);
	UIManager ui(window, initWidth, initHeight);
	ResourceManager::init();
	
	while (!glfwWindowShouldClose(window)) {
		//updating app state
		EngineApp::idleCallback();	
		EngineApp::displayCallback();
		ui.render();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	EngineApp::cleanUp();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

