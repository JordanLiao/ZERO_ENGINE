#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include <iostream>
#include <stdio.h>
#include "tools/Imgui/imgui.h"
#include "tools/Imgui/imgui_impl_opengl3.h"
#include "tools/Imgui/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"

class UIManager {
private:
	GLFWwindow* window;
	bool showDemo;
	bool showAnotherWindow;
	ImVec4 clear_color;
public:
	UIManager();
	UIManager(GLFWwindow* window);
	~UIManager();
	void render();

	//all the ui components
	void showTopMenu();
	void showResourceDock();
};

#endif // _UIMANAGER_H_

