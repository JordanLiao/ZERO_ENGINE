#include "ShadowTester.h"

ShadowTester::ShadowTester() {
	Object * ground = ResourceManager::loadObject("Assets/ground.obj");
	Instance* groundInst = new Instance(ground);
	glm::mat4 trans = glm::translate(glm::vec3(0.f, 0.f, 0.f));
	groundInst->model = trans * groundInst->model;
	
	Object * pine = ResourceManager::loadObject("Assets/lowpolypine.obj");
	Instance* pineInst = new Instance(pine);
	trans = glm::translate(glm::vec3(10.f, 0.f, 0.f));
	pineInst->model = trans * pineInst->model;

	insts.push_back(groundInst);
	insts.push_back(pineInst);
	light = new LightSource(glm::vec3(10.f, 10.f, 0.f), glm::vec3(-1.f, -0.5f, 0.f), lightType::directional, glm::vec3(0.9f, 0.9f, 0.9f));
}

void ShadowTester::render() {
	glm::mat4 currModel = glm::mat4(1.f);

	Renderer::useShadowMap(true);
	glcheck(glClear(GL_DEPTH_BUFFER_BIT));
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < insts.size(); i++) {
		Renderer::drawInstance(insts[i], currModel, shadowMapShader, light);
	}
	Renderer::useShadowMap(false);

	// in the future scene needs to decide which shader to draw which instance
	//Renderer::setLight(light, phongShader);
	for (int i = 0; i < insts.size(); i++) {
		//Renderer::drawInstance(insts[i], currModel, phongShadowShader, light);
	}

	Renderer::drawShadowInspection();
}
