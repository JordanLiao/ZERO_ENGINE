#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <string>
#include <unordered_map>

#include "../Core.h"
#include "Mesh.h"

class Object {
private:
	GLuint vao;
	std::vector<GLuint> vbo;
	GLuint ebo;

public:
	std::vector<Mesh*> meshList; //might want to use a different data structure for efficiencys
	std::string mtlFileName; //store the mtl file name in case the file is not yet loaded.
	std::string objFileName;
	std::unordered_map<std::string, Resources::Material*> matMap; //map of all the materials this Object uses

	Object(std::string& objName, GLuint vertArrObj = 0);
	Object(std::string& objName, std::string& mtlName, std::vector<Mesh*>& meshes, 
	std::unordered_map<std::string, Resources::Material*>& materials, GLuint vertArrObj = 0);
	~Object();
	void render();
	
	// these two functions are used when meshlist and matmap are not provided during construction
	void setMeshList(std::vector<Mesh*>&);
	void setMatMap(std::unordered_map<std::string, Resources::Material*>&);
	void setMtlName(std::string& mtlName);
	void setVao(GLuint);
	void setEbo(GLuint);
	void setVbo(GLuint* vbo, int size);
	//

};

#endif

