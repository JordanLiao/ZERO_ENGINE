#include "LightSource.h"

LightSource::LightSource(glm::vec3 pos, glm::vec3 dir, lightType t) {
	position = pos;
	direction = dir;
	type = t;

	if (t == directional) {
		view = glm::lookAt(glm::vec3(0.f) - glm::normalize(dir) * 1000.f, glm::vec3(0.0f), glm::vec3(0.f, 1.f,0.f));
	}
	else if(t == point){
		view = glm::lookAt(pos, pos + dir, glm::vec3(0.f, 1.f, 0.f));
	}
	float near_plane = 1.0f, far_plane = 30.f;
	proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
	projView = proj * view;
}