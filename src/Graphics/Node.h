#ifndef _NODE_H_
#define _NODE_H_

#include "../Core.h"
#include "../tools/ColorID.h"
#include "../tools/MathConverter.h"

#include <vector>
#include <string>
#include <assimp/scene.h>

class Node {
public:
	std::string nodeName;
	glm::mat4 model;
	std::vector<int> meshID; //stores the id of meshes in this node
	std::vector<Node*> children;

	Node();
	/*
		build a Node Tree based on the given aiNode structure
	*/
	Node(aiNode* node);
	~Node();

	//find the first node that has the argument nodeName starting from this node
	Node* find(std::string& nodeName);

private:
	//the initial transformation model
	glm::mat4 initialModel;
};

#endif // !NODE_H_
