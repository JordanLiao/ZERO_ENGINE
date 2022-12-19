#ifndef _NODE_H_
#define _NODE_H_

#include "Mesh.h"
#include "../Core.h"

#include <vector>

class Node {
public:
	std::vector<Mesh*> meshList;
	std::vector<Node*> children;
	glm::mat4 model;

	Node(glm::mat4 = glm::mat4(1));
};

#endif