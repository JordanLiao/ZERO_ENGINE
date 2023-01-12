#include "InspectionPlane.h"

InspectionPlane::InspectionPlane() {
	obj= ResourceManager::loadObject("Assets/InspectionPlane.obj");
	if (obj == nullptr) {
		inst = new Instance(obj);
		std::cerr << "Cannot create inspection plane object." << std::endl;
	}
	inst = new Instance(obj);

	model = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.f, 0.f, 100.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	float near_plane = 0.1f, far_plane = 120.f;
	proj = glm::ortho(-110.0f, 110.0f, -110.0f, 110.0f, near_plane, far_plane);
	//proj = glm::perspective(glm::radians(80.0), 1920.0/1080.0, 1.0, 400.0);
	projView = proj * view;
}
