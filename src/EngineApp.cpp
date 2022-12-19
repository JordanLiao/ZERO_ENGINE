#include "EngineApp.h"

GLFWwindow* EngineApp::window;

//current moused picked object
Instance* EngineApp::focusedInstance = NULL;

//mouse/keyboard states
bool EngineApp::mousePressed[3];
cursorState EngineApp::currentCursorState = cursorState::idle;
double EngineApp::cursorPosition[2] = {0.0, 0.0};
double EngineApp::prevCursorPosition[2] = {0.0, 0.0};
bool EngineApp::keyPressed[350];

/************************************************************************************************
	NOTE!!:
		Static objects need to have heap allocation using pointer because not every dependency might
	be initialized at the beginning if the program were to left to init them on its own.
		Declare pointers down here, and init them in the initializeObject() function.
*/
Scene* EngineApp::currScene;
//ShadowTester * EngineApp::shadowTest;
/************************************************************************************************/

bool EngineApp::initializeProgram(GLFWwindow* w) {
	window = w;
	Shader* simpleShader = new Shader("src/shaders/shader.vert", "src/shaders/shader.frag");
	Shader* phongTextureShader = new Shader("src/shaders/phong.vert", "src/shaders/phong.frag");
	Shader* colorCodeshader = new Shader("src/shaders/colorPicking.vert", "src/shaders/colorPicking.frag");
	Shader* shadowShader = new Shader("src/shaders/shadow.vert", "src/shaders/shadow.frag");
	Shader* phongTextureShadowShader = new Shader("src/shaders/phongShadow.vert", "src/shaders/phongShadow.frag");
	Shader* shadowInspectionShader = new Shader("src/shaders/phong.vert", "src/shaders/shadowInspection.frag");
	
	Renderer::addShader(shaderRole::phongShader, phongTextureShader);
	Renderer::addShader(shaderRole::simpleModelShader, simpleShader);
	Renderer::addShader(shaderRole::colorPickingShader, colorCodeshader);
	Renderer::addShader(shaderRole::shadowMapShader, shadowShader);
	Renderer::addShader(shaderRole::phongShadowShader, phongTextureShadowShader);
	Renderer::addShader(shaderRole::shadowInspectionShader, shadowInspectionShader);

	return true;
}

bool EngineApp::initializeObjects() {
	currScene = new Scene();
	//shadowTest = new ShadowTester();

	return true;
}

void EngineApp::idleCallback() {
	// Perform any necessary updates here
	if (currentCursorState == cursorState::freeCam) { //move camera in free cam
		if (keyPressed[GLFW_KEY_W])
			moveCamera(GLFW_KEY_W);
		if (keyPressed[GLFW_KEY_A])
			moveCamera(GLFW_KEY_A);
		if (keyPressed[GLFW_KEY_S])
			moveCamera(GLFW_KEY_S);
		if (keyPressed[GLFW_KEY_D])
			moveCamera(GLFW_KEY_D);
		if (keyPressed[GLFW_KEY_LEFT_SHIFT])
			moveCamera(GLFW_KEY_LEFT_SHIFT);
		if (keyPressed[GLFW_KEY_LEFT_CONTROL])
			moveCamera(GLFW_KEY_LEFT_CONTROL);
	}
}

void EngineApp::displayCallback() {
	currScene->render(glm::mat4(1));
	//shadowTest->render();
}

/*
	handle color picking:
	if mouse left click on any displayed object, set that object as the current focusedInstance
*/
void EngineApp::colorPick(double x, double y) {
	currScene->renderColorCode(glm::mat4(1));
	glm::vec4 colorCode = Window::getPixel1Value(framebuffer::pickingFrame, (int)x, (int)y);
	int code = ((int)colorCode.x << 16) + ((int)colorCode.y << 8) + (int)colorCode.z;
	//std::cout << code << std::endl;
	focusedInstance = currScene->getInstanceFromColorCode(code); 
}

void EngineApp::moveCamera(int key) {
	Camera* cam = Renderer::getCurrCamera();
	if (key == GLFW_KEY_W)
		cam->translate(cam->lookDirection * 0.07f);
	else if (key == GLFW_KEY_A)
		cam->translate(cam->camLeft * 0.05f);
	else if (key == GLFW_KEY_S)
		cam->translate(cam->lookDirection * -0.07f);
	else if (key == GLFW_KEY_D)
		cam->translate(cam->camRight * 0.05f);
	else if (key == GLFW_KEY_LEFT_SHIFT)
		cam->translate(glm::vec3(0.f, 0.05f, 0.0f));
	else if (key == GLFW_KEY_LEFT_CONTROL)
		cam->translate(glm::vec3(0.f, -0.05f, 0.0f));
}

void EngineApp::cleanUp() {

}

Scene* EngineApp::getCurrentScene()
{
	return currScene;
}

void EngineApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		keyPressed[key] = true;
		switch (key) {
		case GLFW_KEY_ESCAPE:
			//glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_G: //enables object picking
			if (focusedInstance != NULL) {
				if (currentCursorState == cursorState::idle) {
					currentCursorState = cursorState::picking; //go into mouse/cursor picking mode
					//std::cout << "Cursor state is now picking." << std::endl;
				}
				else if (currentCursorState == cursorState::picking) { // revert cursor state to idle
					currentCursorState = cursorState::idle;
					focusedInstance = NULL;
				}
			}
			break;
		case GLFW_KEY_F: //free cam that allows camera rotation and movement
			if (currentCursorState == cursorState::idle) {
				currentCursorState = cursorState::freeCam;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else if (currentCursorState == cursorState::freeCam) { //revert free cam to idle fix cam
				currentCursorState = cursorState::idle;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		keyPressed[key] = false;
	}
}

void EngineApp::cursor_callback(GLFWwindow* window, double currX, double currY) {
	double dx = currX - cursorPosition[0];
	double dy = currY - cursorPosition[1];
	
	Camera* cam = Renderer::getCurrCamera();
	glm::vec3 relativePoint(0.0f); //camera is relative to the origin if no object is focused on
	if (focusedInstance != NULL)
		relativePoint = focusedInstance->pos;
	float dis = glm::length(glm::dot((relativePoint - cam->eyePos), cam->lookDirection));

	switch (currentCursorState) {
	case cursorState::picking: {
		focusedInstance->translate((cam->camRight * float(dx) - cam->camUp * float(dy)) /
			SLOW_DOWN_FACTOR * dis / REF_DISTANCE);
		}
		break;
	case cursorState::panning: {
		glm::vec3 offset = (cam->camRight * float(dx) - cam->camUp * float(dy)) /
			SLOW_DOWN_FACTOR * dis / REF_DISTANCE;
		cam->translate(offset);
		}
		break;
	case cursorState::freeCam : {
		cam->rotate(cam->camUp, dx * 0.003);
		cam->rotate(cam->camLeft, dy * -0.003);
	}
	default:
		break;
	}

	cursorPosition[0] = currX;
	cursorPosition[1] = currY;
}

void EngineApp::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) { // if left mouse button pressed
		if(button == GLFW_MOUSE_BUTTON_LEFT) { // if left mouse button pressed
			mousePressed[0] = true;
			switch (currentCursorState) {
			case cursorState::idle:
				colorPick(cursorPosition[0], cursorPosition[1]);
				
				//also in addition to color pick, print the depth map's depth value
				std::cout << "depth map pixel value: " << Window::getPixelDepthValue(framebuffer::shadowMapFrame, 
				cursorPosition[0], cursorPosition[1]) << std::endl;

				//print the pixel's color value
				//glm::vec4 color = Window::getPixel1Value(framebuffer::defaultFrame, cursorPosition[0], cursorPosition[1]);
				//std::cout << cursorPosition[0] << " " << cursorPosition[1] << " : " << color[0] << " " << color[1] << " " <<
					//color[2] << std::endl;
				
				break;
			case cursorState::picking:
				currentCursorState = cursorState::idle;
				focusedInstance = NULL;
				break;
			default:
				break;
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) { // if right mouse button pressed
			mousePressed[1] = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {  // scroll wheel pressed
			mousePressed[2] = true;
			if (currentCursorState == cursorState::idle) { // only go to panning from idle state
				currentCursorState = cursorState::panning;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}

	//release actions
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			mousePressed[0] = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			mousePressed[1] = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			mousePressed[2] = false;
			currentCursorState = cursorState::idle;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void EngineApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (currentCursorState != cursorState::picking) { // can only zoom in and out while not scrolling
		Camera* cam = Renderer::getCurrCamera();
		cam->zoom((float)yoffset / 2.f);
	}
}

