#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "../Core.h"
#include "../Graphics/Instance.h"
#include "LightSource.h"
#include "../Renderer/Renderer.h"

class Scene {
private:
	//scene transforms
	glm::vec3 translation;
	glm::mat4 rotation;
	float scaleAmount;
	glm::mat4 model; // scene-wise model matrix that would be applied upon every objects and subscenes

	LightSource* light; //may want to make this an vector in the futrue to have multiple light sources
	std::vector<Scene*> subScenes;
	std::vector<Instance*> instances; //instances of objects on the current level
	//mapping color id to instances
	std::unordered_map<int, Instance*> colorCodeMap;
public:
	Scene();
	~Scene();
	void render(glm::mat4 m);
	void renderColorCode(glm::mat4 m);
	void translate(glm::vec3);
	void rotate(glm::mat4);
	void scale(float);
	void addInstance(Instance*);
	Instance* getInstanceFromColorCode(int code);
	std::vector<Instance*> getInstanceList();
};

#endif