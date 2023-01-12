
#ifndef _OBJECT_H_9
#define _OBJECT_H_

#include <vector>
#include <string>
#include <unordered_map>

#include "../Core.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"
#include "Node.h"
#include "Bone.h"

class Object{
	//TODO: might need these following to release buffer on the GPU side
	std::vector<GLuint> vbo;
	GLuint ebo;

public:
	GLuint vao;
	/*list of bone info; 
	the indices of this list are the bone IDs. During animation the bones transformation
	matrices will be contructed using this list. */
	std::vector<Bone*> boneDataList;
	std::unordered_map<std::string, unsigned int> boneNameToID; //IMPORTANT: boneID is 1th indexed.

	std::vector<SkeletalAnimation*> animations;
	std::unordered_map<std::string, std::size_t> animationMap; //maps animation name to its index

	glm::mat4 model;
	Node* root; //the root node of this Object's scene graph; the root may not have any mesh
	std::unordered_map<std::string, Resources::Material*> matMap; //map of all the materials this Object uses
	std::vector<Mesh*> meshList; //might want to use a different data structure for efficiencys
	std::string objFileName;

	Object(std::string& objName, GLuint vertArrObj = 0, glm::mat4 m = glm::mat4(1));
	~Object();
	
	// setter functions
	void setMatMap(std::unordered_map<std::string, Resources::Material*>&);
	void setVao(GLuint);
};

#endif

