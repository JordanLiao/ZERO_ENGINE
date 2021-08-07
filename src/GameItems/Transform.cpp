#include "Transform.h"

Transform::Transform() {
	translation = glm::vec3(0);
	rotation = glm::rotate(0.0f, glm::vec3(0.f, 1.f, 0.f)); // not rotating
	scaleAmount = 1.0f;
	model = glm::mat4(1);
}

void Transform::render(glm::mat4 parentModel) {
	std::list<SceneNode*>::iterator it = children.begin();
	while (it != children.end()) {
		(*it)->render(parentModel * model);
		it++;
	}
}

void Transform::addChild(SceneNode* child){
	children.push_back(child);
}

void Transform::translate(glm::vec3 offset) {
	translation += offset;
	model *= glm::translate(offset);
}

void Transform::rotate(glm::mat4 rot) {
	rotation = rot * rotation;
	model = glm::translate(translation) * rotation * glm::scale(glm::vec3(scaleAmount));
}

void Transform::scale(float s) {
	scaleAmount = s;
	model = glm::translate(translation) * rotation * glm::scale(glm::vec3(scaleAmount));
}
