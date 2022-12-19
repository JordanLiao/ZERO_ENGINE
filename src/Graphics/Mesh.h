#ifndef _MESH_H_
#define _MESH_H_

#include "../Core.h"
#include "../ResourceManagement/Resources.h"

class Mesh {
public:
	std::string meshName;
	Resources::Material* material;
	GLuint indexOffset; //offset in terms of indices(not vertices) into vao to begin drawing this mesh
	/*size is number of vertices referenced by this mesh's indices; NOTE that this is not the num of vertices
	in this mesh because identical vertices are joined; its value is 3 * num triangles*/
	int size; 

	/*
		name: name of the mesh.
		mtl: reference to the material for this mesh.
		indexOff: offset in terms of indices(not vertices) into VAO to begin drawing this mesh
		meshSize: number of vertices referenced by this mesh's indices, 3 * num triangles
	*/
	Mesh(std::string& name, Resources::Material* mtl, int indexOff, int meshSize);
	~Mesh();
	std::string& getMeshName();
	bool setMaterial(Resources::Material*);
	Resources::Material* getMaterial();
};

#endif
