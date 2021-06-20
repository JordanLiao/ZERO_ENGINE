#ifndef _CORE_H_
#define _CORE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void clearError();
bool checkError(const char*, const char*, int);

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define glcheck(stmt) clearError(); \
	stmt;\
	ASSERT(checkError(#stmt, __FILE__, __LINE__))
#else
#define glcheck(stmt) stmt
#endif

#endif