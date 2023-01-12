#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Core.h"
#include "Shader.h"
#include "../GameItems/Camera.h"
#include "../GameItems/LightSource.h"
#include "../Window.h"
#include "../Graphics/Instance.h"
#include "../Graphics/InspectionPlane.h"
#include "../Graphics/SkeletalAnimation.h"

class Renderer {
private:
	static Shader* shaders[];
	static Camera* cameras[]; // TODO NOTE, possibily not a good choice to keep cameras list in the renderer class becausae
	static Camera* currCamera; // cameras are often associated with scenes and light sources.
	static LightSource* currLight;
	static InspectionPlane * inspectionPlane;

	static void getBoneTransformation(std::vector<glm::mat4>& boneTransformation, Node* node, glm::mat4& globalInverseModel,
		       glm::mat4& parentModel, std::vector<Bone*>& boneDataList, std::unordered_map<std::string, unsigned int>& boneNameToID);

public:
	static void initRenderer(Camera * engineDisplayCameraq);

	/*
		Draws an instance of an Engine object. 
	*/
	static void drawInstance(Instance * inst, glm::mat4 m, shaderRole s, LightSource * l);

	static void drawAnimatedInstance(Instance* inst, LightSource* light, std::size_t animID);

	/*
		This draw function renders a color coded texture to a separate color picking framebuffer
		which enables the EngineApp to determine which pixel corresponds to which unique color coded
		engine instance.
	*/
	static void drawInstanceToColorPickingFrameBuffer(Instance *inst, glm::mat4 m, int colorCode);

	/*
		This draw function renders the depth information of a scene as a texture to the depth map, which
		enables shadow effect in the scene.
	*/
	static void drawInstanceToShadowMapFrameBuffer(Instance* inst, glm::mat4 m, LightSource *L);

	/*
		This draw function displays the shadow map on a fixed 2d plane on the screen for manual inspection of
		the shadow map.
	*/
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
