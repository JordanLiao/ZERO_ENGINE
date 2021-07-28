#include "EngineApp.h"

GLFWwindow* EngineApp::window;
UIManager EngineApp::ui;

//current moused picked object
Object* EngineApp::focusedObject = NULL;
std::unordered_map<int, Object*> EngineApp::colorCodeMap;

//mouse/keyboard states
bool EngineApp::leftMousePressed = false;
bool EngineApp::rightMousePressed = false;
cursorState EngineApp::currentCursorState = cursorState::idle;
double EngineApp::cursorPosition[2] = {0.0, 0.0};
double EngineApp::prevCursorPosition[2] = {0.0, 0.0};
bool EngineApp::keyPressed[350];

Object* EngineApp::obj;

void EngineApp::idleCallback() {
	// Perform any necessary updates here 
}

void EngineApp::displayCallback()
{
	obj->render();

	ui.render();
}

/*
	handle color picking:
	--if mouse left click on any displayed object, set that object as the current focusedObject
*/
void EngineApp::colorPick(double x, double y) {
	obj->renderColorCode();
	glm::vec4 colorCode = Window::getPixel1Value(framebuffer::pickingFrame, (int)x, (int)y);
	int code = ((int)colorCode.x << 16) + ((int)colorCode.y << 8) + (int)colorCode.z;
	std::cout << code << std::endl;
	if (colorCodeMap.find(code) != colorCodeMap.end())
		focusedObject = colorCodeMap[code]; 
}

bool EngineApp::initializeProgram(GLFWwindow * w) {
	window = w;
	ui = UIManager(w);
	Shader* shader = new Shader("src/shaders/phongTexture.vert", "src/shaders/phongTexture.frag");
	Shader* colorCodeshader = new Shader("src/shaders/colorPicking.vert", "src/shaders/colorPicking.frag");
	// Check the shader program.
	if (shader->getId() == 0)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	Renderer::addShader(shaderRole::phongShader, shader);
	Renderer::addShader(shaderRole::colorPickingShader, colorCodeshader);
	Renderer::setShader(shaderRole::phongShader); // set phong shading as default

	return true;
}

bool EngineApp::initializeObjects() {
	obj = ResourceManager::loadObject("Assets/lowpolypine.obj");
	colorCodeMap[obj->colorId] = obj;
	return true;
}

void EngineApp::cleanUp() {

}

void EngineApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			//glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_G:
			if (focusedObject != NULL) {
				if (currentCursorState == cursorState::idle) {
					currentCursorState = cursorState::picking; //go into mouse/cursor picking mode
					std::cout << "Cursor state is now picking." << std::endl;
				}
				else if (currentCursorState == cursorState::picking) { // revert cursor state to idle
					currentCursorState = cursorState::idle;
					focusedObject = NULL;
				}
			}
			break;
		default:
			break;
		}
	}
}

void EngineApp::cursor_callback(GLFWwindow* window, double currX, double currY) {
	double dx = currX - cursorPosition[0];
	double dy = currY - cursorPosition[1];

	if (currentCursorState == cursorState::picking) {
		Camera* cam = Renderer::getCurrCamera();
		float dis = glm::length(glm::dot((focusedObject->pos - cam->eyePos), cam->lookDirection));
		//std::cout << "dis " << dis << std::endl;
		focusedObject->translate((cam->camRight * float(dx) - cam->camUp * float(dy)) / 
		SLOW_DOWN_FACTOR * dis / REF_DISTANCE);
	}

	cursorPosition[0] = currX;
	cursorPosition[1] = currY;
}

void EngineApp::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) { // if left mouse button pressed
		if(button == GLFW_MOUSE_BUTTON_LEFT) { // if left mouse button pressed
			leftMousePressed = true;
			switch (currentCursorState) {
			case cursorState::idle:
				colorPick(cursorPosition[0], cursorPosition[1]);
				break;
			case cursorState::picking:
				currentCursorState = cursorState::idle;
				focusedObject = NULL;
				break;
			default:
				break;
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) { // if right mouse button pressed
			rightMousePressed = true;
		}
	}

	//release actions
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			leftMousePressed = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			rightMousePressed = false;
		}
	}
}

void EngineApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (currentCursorState != cursorState::picking) { // can only zoom in and out while not scrolling
		Camera* cam = Renderer::getCurrCamera();
		cam->zoom((float)yoffset / 2.f);
	}
}

