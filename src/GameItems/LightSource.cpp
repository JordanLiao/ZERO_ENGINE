#include "LightSource.h"

LightSource::LightSource(glm::vec3 pos, glm::vec3 dir, lightType t, glm::vec3 c) {
	position = pos;
	direction = dir;
	type = t;
	color = c;

	float nearPlane = 0.1f, farPlane = 40.f;
	if (t == directional) {
		view = glm::lookAt(glm::vec3(0.f) - glm::normalize(dir) * 30.f, glm::vec3(0.0f), glm::vec3(0.f, 1.f,0.f));
		proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	}
	else if(t == point){
		view = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.f, 1.f, 0.f));
		proj = glm::perspective(glm::radians(90.0), (double)1.0, (double)nearPlane, (double)farPlane);
	}
	projView = proj * view;
}