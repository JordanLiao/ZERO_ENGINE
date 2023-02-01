#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
int Window::shadowTextureSize = 1600;
const char* Window::windowTitle = "ZeroEngine App";

//projection matrix
double Window::near = 0.1;
double Window::far = 1000.0;
double Window::fov = 50.0;
glm::mat4 Window::projection = glm::mat4(1); // initially set to the identity matrix because window size is unknown, resizecallback()
                                             // would give the projection matrix an actual value.

//color picking offscreen framebuffer properties
GLuint Window::pickingFramebuffer, Window::pickingRenderbuffer, Window::pickingDepthbuffer;
//shadow map
GLuint Window::shadowMapFramebuffer, Window::shadowMapDepthTexture;

GLFWwindow* Window::createWindow(int initWidth, int initHeight) {
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
	//create shadow map framebuffer
	glcheck(glGenFramebuffers(1, &shadowMapFramebuffer));

	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer));
	glcheck(glGenTextures(1, &shadowMapDepthTexture));
	glcheck(glBindTexture(GL_TEXTURE_2D, shadowMapDepthTexture));
	glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowTextureSize, shadowTextureSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	//glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE));
	//glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	//glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	//glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glcheck(glBindTexture(GL_TEXTURE_2D, 0));
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer));
	glcheck(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapDepthTexture, 0));
	glcheck(glDrawBuffer(GL_NONE));
	glcheck(glReadBuffer(GL_NONE));
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0)); //unbind the picking framebuffer

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, initWidth, initHeight);

	//check if picking buffer is complete
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Error" << '\n';
	}
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Error" << '\n';
	}
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return window;
}

//updating framebuffers if the screen size changes
void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	//if window is minimized, do nothing to the projection matrix, else the app would crash.
	if (width <= 0 || height <= 0)
		return;

	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glcheck(glViewport(0, 0, width, height));
	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov),
		double(width) / (double)height, near, far);

	//update the renderbuffer attached to the offscreen framebuffers
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer)); //bind to the picking framebuffer
	glcheck(glDeleteRenderbuffers(1, &pickingRenderbuffer)); //delete old renderbuffer
	glcheck(glDeleteRenderbuffers(1, &pickingDepthbuffer)); //delete old depthbuffer
	
	glcheck(glGenRenderbuffers(1, &pickingRenderbuffer));  //generate new renderbuffer
	glcheck(glBindRenderbuffer(GL_RENDERBUFFER, pickingRenderbuffer)); //bind the renderbuffer
	glcheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height)); //allocate storage
	glcheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, pickingRenderbuffer)); //attach the renderbuffer
	
	glcheck(glGenRenderbuffers(1, &pickingDepthbuffer));  //generate new depthbuffer
	glcheck(glBindRenderbuffer(GL_RENDERBUFFER, pickingDepthbuffer));
	glcheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	glcheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepthbuffer));
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0)); //unbind the picking framebuffer
	//---------------------------------------------------//
	
}

void Window::bindFramebuffer(framebuffer id) {
	//std::cout << "binding framebuffer" << std::endl;
	if (id == framebuffer::defaultFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glcheck(glViewport(0, 0, width, height));
	}
	else if (id == framebuffer::pickingFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
		glcheck(glViewport(0, 0, width, height));
	}
	else if (id == framebuffer::shadowMapFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer));
		glcheck(glViewport(0, 0, shadowTextureSize, shadowTextureSize));
	}
}

glm::vec4 Window::getPixel1Value(framebuffer frame, int x, int y)
{
	unsigned char data[4];
	if (frame == framebuffer::defaultFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	else if (frame == framebuffer::pickingFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	}
	glcheck(glReadPixels(x, height - y, 1, 1, GL_BGRA, GL_UNSIGNED_BYTE, data));
	// Return to onscreen rendering:
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return glm::vec4((int)data[0], (int)data[1], (int)data[2], (int)data[3]);
}

float Window::getPixelDepthValue(framebuffer frame, int x, int y) {
	if (frame == framebuffer::defaultFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	else if (frame == framebuffer::pickingFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, pickingFramebuffer));
	}
	else if (frame == framebuffer::shadowMapFrame) {
		glcheck(glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer));
	}
	
	float data;
	glcheck(glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &data));
	glcheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	return data;
}

