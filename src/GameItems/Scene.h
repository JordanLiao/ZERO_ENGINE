#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "../Core.h"
#include "Transform.h"

enum lightType {
	point,
	directional,
	spot
};

typedef struct LightSource {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	lightType type;

	LightSource(glm::vec3 pos, glm::vec3 dir, glm::vec3 c, lightType type);
} LightSource;

class Scene {
private:
	LightSource* light; //may want to make this an vector in the futrue to have multiple light sources
	Transform* topTransform;
public:
	Scene();
	~Scene();
	void render();
};

#endif