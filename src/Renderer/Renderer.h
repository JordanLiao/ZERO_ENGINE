#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Core.h"
#include "Shader.h"
#include "../GameItems/Camera.h"
#include "../Window.h"
#include "../GameItems/LightSource.h"
#include "../GameItems/Instance.h"
#include "../Renderer/InspectionPlane.h"

class Renderer {
private:
	static Shader* shaders[];
	static Camera* cameras[]; // TODO NOTE, possibily not a good choice to keep cameras list in the renderer class becausae
	static Camera* currCamera; // cameras are often associated with scenes and light sources.
	static LightSource* currLight;
	static InspectionPlane * inspectionPlane;

public:
	static void initRenderer(Camera * engineDisplayCameraq);

	static void draw(glm::mat4 model, Resources::Material* mat, GLuint vao, int offset, int count, shaderRole sRole);
	static void drawInstance(Instance * inst, glm::mat4 m, shaderRole s, LightSource * L);
	static void drawToColorPickingFrameBuffer(glm::mat4 model, GLuint vao, int offset, int count, int colorCode);
	static void drawInstanceToColorPickingFrameBuffer(Instance *inst, glm::mat4 m, int colorCode);
	static void drawInstanceToShadowMapFrameBuffer(Instance* inst, glm::mat4 m, LightSource *L);
	static void drawShadowInspection();
	
	static bool prepMaterial(Resources::Material* mat, Shader * s);
	static void addShader(shaderRole shaderType, Shader * s);
	static void addCamera(cameraRole cRole, Camera* cam);
	static bool setCamera(cameraRole cRole);
	static void setLight(LightSource * light, shaderRole sRole);
	static void useShadowMap(bool);

	static Camera* getCurrCamera();
};

#endif
