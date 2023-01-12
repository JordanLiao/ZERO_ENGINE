#ifndef _ENGINEAPP_H_
#define _ENGINEAPP_H_

#include <unordered_map>
#include <chrono> 

#include "Renderer/Renderer.h"
#include "GameItems/Scene.h"
#include "ResourceManagement/ResourceManager.h"

//pre-determined constants for dragging to ensure that when camera zooms out, 
//dragging speed is increased proportionally.
const float SLOW_DOWN_FACTOR = 61.5f;
const float REF_DISTANCE = 9.8387f;

enum class cursorState {
	idle,
	picking, // when G is pressed
	panning,  // when scroll wheel is hold down
	freeCam
};

class EngineApp {
private:
	static GLFWwindow* window;

	static Scene* currScene;
	static Instance* focusedInstance;

	//mouse/keyboard states
	static bool mousePressed[3];
	static cursorState currentCursorState;
	static double cursorPosition[2];
	static double prevCursorPosition[2];
	static bool keyPressed[350];

	static uint64_t appStartTime, appCurrTime, appDuration;

	/*******************/
	//static ShadowTester * shadowTest;
	/*******************/

public:
	//application callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback();

	//handler functions
	static void colorPick(double x, double y);
	static void moveCamera(int key);

	//initialization and clean up functions
	static bool initializeProgram(GLFWwindow * w);
	static bool initializeObjects();
	static void cleanUp();

	//getter/setters
	static Scene* getCurrentScene();
};

#endif