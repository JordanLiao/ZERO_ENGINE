#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Core.h"
#include "Shader.h"
#include "Resources.h"
#include "Camera.h"
#include "../Window.h"

class Renderer {
private:
	static Shader* shaders[];
	static Shader* currShader;
	static Camera* cameras[];
	static Camera* currCamera;
public:
	static void initRenderer(Camera * engineDisplayCameraq);

	static void draw(glm::mat4 model, Resources::Material* mat, GLuint vao, int offset, int count);
	static void drawToColorPickingFrameBuffer(glm::mat4 model, GLuint vao, int offset, int count, int colorCode);
	static bool prepShader(Resources::Material* mat);
	static bool setColorCoding(int code);
	static void addShader(shaderRole shaderType, Shader * s);
	static bool setShader(shaderRole sRole);
	static void addCamera(cameraRole cRole, Camera* cam);
	static bool setCamera(cameraRole cRole);

	static Camera* getCurrCamera();
};

#endif
