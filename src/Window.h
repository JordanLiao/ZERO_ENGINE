#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Core.h"

enum class framebuffer {
	defaultFrame, //GLFW managed framebuffer
	pickingFrame, //application managed framebuffer for model picking
	shadowMapFrame
};

class Window
{
public:
	static GLuint shadowMapDepthTexture;

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static int shadowTextureSize;

	static double near, far;
	static glm::mat4 projection;

	// Window functions
	static GLFWwindow* createWindow(int initWidth, int initHeight);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void bindFramebuffer(framebuffer id);

	//value access functions
	static glm::vec4 getPixel1Value(framebuffer frame, int x, int y);
	static float getPixelDepthValue(framebuffer frame, int x, int y);

private:
	//color picking offscreen framebuffer properties
	static GLuint pickingFramebuffer, pickingRenderbuffer, pickingDepthbuffer;
	static GLuint shadowMapFramebuffer;
};

#endif