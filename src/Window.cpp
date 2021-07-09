#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

//projection matrix
glm::mat4 Window::projection = glm::mat4(1);

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 7);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Renderer tools
Shader* Window::shader;
Renderer Window::renderer;
ResourceManager Window::resourceManager;

//testing obj
Object* Window::obj;

bool Window::initializeProgram() {
	shader = new Shader("src/shaders/phongTexture.vert", "src/shaders/phongTexture.frag", &projection, &view);
	//shader = new Shader("src/shaders/shader.vert", "src/shaders/shader.frag", &projection, &view);
	obj = resourceManager.loadObject("Assets/lowpolypine.obj");
	renderer.setShader(shader);
	obj->setRenderer(&renderer);

	// Check the shader program.
	if (shader->getId() == 0)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	return true;
}

void Window::cleanUp()
{
}

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

void Window::idleCallback()
{
	// Perform any necessary updates here 
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glcheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//render here
	obj->render();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	/*if (button == GLFW_MOUSE_BUTTON_LEFT) {
		std::cout << "mouse_callback is not currently used" << std::endl;
	}*/
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
	//std::cout << "cursor_callback not currently used" << std::endl;
}
