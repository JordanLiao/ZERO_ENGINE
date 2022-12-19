#include "Scene.h"

Scene::Scene() {
	translation = glm::vec3(0);
	rotation = glm::rotate(0.0f, glm::vec3(0.f, 1.f, 0.f)); // not rotating
	scaleAmount = 1.0f;
	model = glm::mat4(1);

	light = new LightSource(glm::vec3(10.f, 10.f, 0.f), glm::vec3(-1.f, -1.f, 0.f), lightType::directional, glm::vec3(0.9f, 0.9f, 0.9f));
	//light = new LightSource(glm::vec3(10.f, 10.f, 0.f), glm::vec3(-1.f, -1.f, 0.f), lightType::point, glm::vec3(0.9f, 0.9f, 0.9f));

	Object* cubes = ResourceManager::loadObject("Assets/cubes.obj");

	Object* ground = ResourceManager::loadObject("Assets/ground.obj");
	Instance* groundInst = new Instance(ground);
	
	Object* lowpolypine = ResourceManager::loadObject("Assets/lowpolypine.obj");
	Object* lowpolypillar = ResourceManager::loadObject("Assets/lowpolypillar.obj");
	
	glm::mat4 trans = glm::translate(glm::vec3(0.f, 0.f, 0.f));
	groundInst->model = trans * groundInst->model;
	colorCodeMap[groundInst->colorId] = groundInst;
	instances.push_back(groundInst);
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
	glm::mat4 currModel = m * model;
	
	//prep shadow map
	Window::bindFramebuffer(framebuffer::shadowMapFrame);
	glcheck(glClear(GL_DEPTH_BUFFER_BIT));
	//glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < instances.size(); i++) {
		Renderer::drawInstance(instances[i], currModel, shadowMapShader, light);
	}
	Window::bindFramebuffer(framebuffer::defaultFrame);
	

	// in the future scene needs to decide which shader to draw which instance
	glcheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	for (int i = 0; i < instances.size(); i++) {
		Renderer::drawInstance(instances[i], currModel, phongShadowShader, light);
	}

	//-----------debugging purpose----------//
	//Renderer::drawShadowInspection();
	//--------------------------------------//
}

/*
	render all the instances using their assigned color code
*/
void Scene::renderColorCode(glm::mat4 m) {
	Window::bindFramebuffer(framebuffer::pickingFrame); //bind to the offscreen picking framebuffer
	glcheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glm::mat4 currModel = m * model;
	for (int i = 0; i < instances.size(); i++) {
		Instance* inst = instances[i];
		//std::cout << "color picking" << std::endl;		
		Renderer::drawInstanceToColorPickingFrameBuffer(inst, currModel, inst->colorId);
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
