#include "Scene.h"

Scene::Scene() {
	translation = glm::vec3(0);
	rotation = glm::rotate(0.0f, glm::vec3(0.f, 1.f, 0.f)); // not rotating
	scaleAmount = 1.0f;
	model = glm::mat4(1);

	light = new LightSource(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 10.f), lightType::directional);
}

Scene::~Scene() {
	delete light;
	for (int i = 0; i < subScenes.size(); i++) {
		delete subScenes[i];
	}
	for (int i = 0; i < instances.size(); i++) {
		delete instances[i];
	}
}

void Scene::render(glm::mat4 m) {
	//prep shadow map
	Renderer::useShadowMap(true);
	
	Renderer::useShadowMap(false);
	
	Renderer::setLight(light);
	glm::mat4 currModel = m * model;
	for (int i = 0; i < instances.size(); i++) {
		instances[i]->render(currModel);
	}
}

/*
	render all the instances using their assigned color code
*/
void Scene::renderColorCode(glm::mat4 m) {
	Window::bindFramebuffer(framebuffer::pickingFrame); //bind to the offscreen picking framebuffer
	glcheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glm::mat4 currModel = m * model;
	for (int i = 0; i < instances.size(); i++) {
		instances[i]->renderColorCode(currModel);
	}
	Window::bindFramebuffer(framebuffer::defaultFrame); //switch back the default framebuffer 
}

void Scene::translate(glm::vec3 offset) {
	translation += offset;
	model *= glm::translate(offset);
}

void Scene::rotate(glm::mat4 rot) {
	rotation = rot * rotation;
	model = glm::translate(translation) * rotation * glm::scale(glm::vec3(scaleAmount));
}

void Scene::scale(float s) {
	scaleAmount = s;
	model = glm::translate(translation) * rotation * glm::scale(glm::vec3(scaleAmount));
}

void Scene::addInstance(Instance* instance) {
	if (colorCodeMap.find(instance->colorId) != colorCodeMap.end()) // if instance already is in the scene
		return;
	colorCodeMap[instance->colorId] = instance;
	instances.push_back(instance);
}

Instance* Scene::getInstanceFromColorCode(int code) {
	if (colorCodeMap.find(code) != colorCodeMap.end())
		return colorCodeMap[code];
	return NULL;
}

std::vector<Instance*> Scene::getInstanceList()
{
	return instances;
}
