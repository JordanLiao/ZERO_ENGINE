#include "Node.h"

Node::Node() {
	nodeName = "Not Initialized node";
	model = glm::mat4(1.f);
	initialModel = model;
}

Node::Node(aiNode* node) {
	model = MathConverter::getMat4FromAiMatrix(node->mTransformation);
	initialModel = model;
	nodeName = node->mName.C_Str();
	meshID = std::vector<int>(node->mNumMeshes, 0);

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		meshID[i] = node->mMeshes[i];
	}
	//construct a sub tree for each of node's children.
	for (unsigned i = 0; i < node->mNumChildren; i++) {
		Node* child = new Node(node->mChildren[i]);
		children.push_back(child);
	}
}

Node::~Node() {
	for (size_t i = 0; i < children.size(); i++) {
		delete children[i];
	}
}

Node* Node::find(std::string& name) {
	if (name == nodeName)
		return this;
	for (Node* child : children) {
		Node* res = child->find(name);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}
