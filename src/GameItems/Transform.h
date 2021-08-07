#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <list>

#include "SceneNode.h"

class Transform : public SceneNode {
private:
	glm::vec3 translation;
	glm::mat4 rotation;
	float scaleAmount;
	glm::mat4 model;
	std::list<SceneNode*> children;

public:
	Transform();
	void render(glm::mat4);
	void addChild(SceneNode *);
	void translate(glm::vec3);
	void rotate(glm::mat4);
	void scale(float);
};

#endif