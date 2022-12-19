#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include "tools/Imgui/imgui.h"
#include "tools/Imgui/imgui_impl_opengl3.h"
#include "tools/Imgui/imgui_impl_glfw.h"
#include "Core.h"

#include "EngineApp.h"
#include "ResourceManagement/ResourceManager.h"

namespace UIConstants {
	//constants
	static int leftPaneWidth = 300;
	static int rightPaneWidth = 350;

	static int resourceDockHeight = 500;
	static int sceneEditorHeight = 400;
}

class UIManager {
private:
	GLFWwindow* window;
	int windowHeight, windowWidth;
	ImVec4 clear_color;
public:
	UIManager();
	UIManager(GLFWwindow* window, int width, int height);
	void setWindow(GLFWwindow*);
	~UIManager();
	void render();

	//setters
	void updateWindowSize(int w, int h);

	//all the ui components
	ImVec2 showTopMenu();
	ImVec2 showResourceDock();
	ImVec2 showSceneEditor();
	ImVec2 showInstanceProperties();
};

#endif // _UIMANAGER_H_

