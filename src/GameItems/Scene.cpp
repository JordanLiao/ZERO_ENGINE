#include "Scene.h"

LightSource::LightSource(glm::vec3 pos, glm::vec3 dir, glm::vec3 c, lightType t) {
	position = pos;
	direction = dir;
	color = c;
	type = t;
}

Scene::Scene() {
	light = new LightSource(glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f), glm::vec3(0.7f, 0.7f, 0.7f), lightType::spot);
	topTransform = new Transform();
}

Scene::~Scene() {
}

void Scene::render()
{
}