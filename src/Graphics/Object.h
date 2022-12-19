#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <string>
#include <unordered_map>

#include "../Core.h"
#include "Node.h"
//#include "../Renderer/Renderer.h"

class Object{

public:
	GLuint vao;
	std::vector<GLuint> vbo;
	GLuint ebo;

	Node* root;
	std::vector<Mesh*> meshList; //might want to use a different data structure for efficiencys
	glm::mat4 model;

	std::string mtlFileName; //store the mtl file name in case the file is not yet loaded.
	std::string objFileName;
	std::unordered_map<std::string, Resources::Material*> matMap; //map of all the materials this Object uses

	Object(std::string& objName, GLuint vertArrObj = 0, glm::mat4 m = glm::mat4(1));
	~Object();
	
	// setter functions
	void setMatMap(std::unordered_map<std::string, Resources::Material*>&);
	void setMtlName(std::string& mtlName);
	void setVao(GLuint);
	void setEbo(GLuint);
	void setVbo(GLuint* vbo, int size);
};

#endif

