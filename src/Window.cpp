#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "ZeroEngine App";

//projection matrix
glm::mat4 Window::projection = glm::mat4(1);

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 7);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	//set up opengl using glew
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}

	// Set swap interval to 1 to have vSync.
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	//if window is minimized, do nothing to the projection matrix, else the app would crash.
	if (width <= 0 || height <= 0)
		return;
	// Set the viewport size.
	glcheck(glViewport(0, 0, width, height));
	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(80.0),
		double(width) / (double)height, 1.0, 1000.0);
}

