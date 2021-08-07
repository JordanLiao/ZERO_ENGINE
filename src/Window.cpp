#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "ZeroEngine App";

//projection matrix
double Window::near = .1f;
double Window::far = 1000.f;
glm::mat4 Window::projection = glm::mat4(1);

//legacy code, camera logic used to in window class
// View Matrix:
/*glm::vec3 Window::eyePos(0, 0, 7);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
glm::vec3 Window::camLeft(-1, 0, 0);        //direction to the left of the camera
glm::vec3 Window::camRight(1, 0, 0);        //direction to the right of the camera*/

//color picking offscreen framebuffer properties
GLuint Window::pickingFramebuffer, Window::pickingRenderbuffer;

GLFWwindow* Window::createWindow()
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window.
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int xpos, ypos, workAreawidth, workAreaheight;
	glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &workAreawidth, &workAreaheight);
	GLFWwindow* window = glfwCreateWindow(workAreawidth, workAreaheight, windowTitle, NULL, NULL);
	glfwSetWindowPos(window, xpos, ypos + 30);
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

	//create the picking framebuffer
	glcheck(glGenFramebuffers(1, &pickingFramebuffer));

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, 1920, 1001);

	//check if picking buffer is complete
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Error" << '\n';
	}
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	std::cout << width << " " << height << std::endl;
	//if window is minimized, do nothing to the projection matrix, else the app would crash.
	if (width <= 0 || height <= 0)
		return;
	// Set the viewport size.
	glcheck(glViewport(0, 0, width, height));
	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(80.0),
		double(width) / (double)height, near, far);

	//update the renderbuffer attached to the offscreen framebuffers
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer)); //bind to the picking framebuffer
	glcheck(glDeleteRenderbuffers(1, &pickingRenderbuffer)); //delete old renderbuffer
	glcheck(glGenRenderbuffers(1, &pickingRenderbuffer));  //generate new renderbuffer
	glcheck(glBindRenderbuffer(GL_RENDERBUFFER, pickingRenderbuffer)); //bind the renderbuffer
	glcheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height)); //allocate storage
	glcheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, pickingRenderbuffer)); //attach the renderbuffer
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0)); //unbind the picking framebuffer
}

void Window::bindFramebuffer(framebuffer id) {
	if (id == framebuffer::defaultFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	else if (id == framebuffer::pickingFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	}
}

glm::vec4 Window::getPixel1Value(framebuffer frame, int x, int y)
{
	unsigned char data[4];
	if (frame == framebuffer::defaultFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	else if (frame == framebuffer::pickingFrame) {
		//std::cout << "binding to picking framebuffer" << std::endl;
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	}
	glcheck(glReadBuffer(GL_COLOR_ATTACHMENT0));
	glcheck(glReadPixels(x, height - y, 1, 1, GL_BGRA, GL_UNSIGNED_BYTE, data));
	// Return to onscreen rendering:
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return glm::vec4((int)data[0], (int)data[1], (int)data[2], (int)data[3]);
}

