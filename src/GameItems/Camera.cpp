#include "Camera.h"

Camera::Camera() {
	eyePos = glm::vec3(0.f, 6.f, 8.f);
	lookDirection = glm::normalize(glm::vec3(0.f, -.5, -1.f));
	lookAtPoint = eyePos + lookDirection;
	upVector = glm::vec3(0.f, 1.0f, 0.f);
	camLeft = glm::normalize(glm::cross(upVector, lookAtPoint - eyePos));
	camRight = camLeft * -1.0f;
	camUp = glm::normalize(glm::cross(lookDirection, camLeft));
	camDown = camUp * -1.0f;
	view = glm::lookAt(eyePos, lookAtPoint, upVector);
}

Camera::Camera(glm::vec3 ePos, glm::vec3 lDirection, glm::vec3 uVector) {
	eyePos = ePos;
	lookDirection = glm::normalize(lDirection);
	lookAtPoint = ePos + lDirection;
	upVector = glm::normalize(uVector);
	camLeft = glm::normalize(glm::cross(upVector, lookAtPoint - eyePos));
	camRight = camLeft * -1.0f;
	camUp = glm::normalize(glm::cross(lookDirection, camLeft));
	camDown = camUp * -1.0f;
	view = glm::lookAt(eyePos, lookAtPoint, upVector);
}

Camera::~Camera() {
}

void Camera::translate(glm::vec3 trans) {
	eyePos += trans;
	lookAtPoint = eyePos + lookDirection;
	view = glm::lookAt(eyePos, lookAtPoint, upVector);
}

void Camera::rotate(glm::vec3 axis, float rad) {
	lookDirection = glm::normalize(glm::vec4(lookDirection, 0.0f) * glm::rotate(rad, axis));
	lookAtPoint = eyePos + lookDirection;
	upVector = glm::vec3(0.f, 1.0f, 0.f);
	camLeft = glm::normalize(glm::cross(upVector, lookAtPoint - eyePos));
	camRight = camLeft * -1.0f;
	camUp = glm::normalize(glm::cross(lookDirection, camLeft));
	camDown = camUp * -1.0f;
	view = glm::lookAt(eyePos, lookAtPoint, upVector);
}

void Camera::zoom(float amt) {
	eyePos += amt * lookDirection;
	lookAtPoint = eyePos + lookDirection;
	view = glm::lookAt(eyePos, lookAtPoint, upVector);
}

void Camera::updateView() {
}



