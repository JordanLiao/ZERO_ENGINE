#ifndef _LIGHTSOURCE_H_
#define _LIGHTSOURCE_H_

#include "../Core.h"

enum lightType {
	point,
	directional,
	spot
};

typedef struct LightSource {
	glm::vec3 position;
	glm::vec3 direction;
	lightType type;

	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 projView;

	LightSource(glm::vec3 pos, glm::vec3 dir, lightType type);
} LightSource;

#endif