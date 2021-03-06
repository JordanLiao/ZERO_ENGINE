#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../Core.h"

enum cameraRole {
	engineDisplayCam,
	cameraSize
};

class Camera {
public:
	glm::mat4 view;
	glm::vec3 eyePos, lookDirection, lookAtPoint, upVector, camLeft, camRight, camUp, camDown;

	Camera();
	Camera(glm::vec3 ePos, glm::vec3 lDirection, glm::vec3 uVector);
	~Camera();

	void translate(glm::vec3);
	void rotate(glm::vec3 axis, float rad);
	void zoom(float); // special case of translation that is only along the look direction
	void updateView();
};

#endif
