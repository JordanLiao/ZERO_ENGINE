#include "EngineApp.h"

Renderer EngineApp::renderer;
GLFWwindow* EngineApp::window;
UIManager EngineApp::ui;

void EngineApp::idleCallback() {
	// Perform any necessary updates here 
}

void EngineApp::displayCallback()
{
	ui.render();
}

bool EngineApp::initializeProgram(GLFWwindow * w) {
	window = w;
	ui = UIManager(w);
	/*shader = new Shader("src/shaders/phongTexture.vert", "src/shaders/phongTexture.frag", &projection, &view);
	// Check the shader program.
	if (shader->getId() == 0)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}*/

	return true;
}

bool EngineApp::initializeObjects() {
	return true;
}

void EngineApp::cleanUp() {

}

void EngineApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		default:
			break;
		}
	}
}

void EngineApp::cursor_callback(GLFWwindow* window, double currX, double currY) {
	std::cout << "cursor_callback not currently used" << std::endl;
}

void EngineApp::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		std::cout << "mouse_callback is not currently used" << std::endl;
	}
}

