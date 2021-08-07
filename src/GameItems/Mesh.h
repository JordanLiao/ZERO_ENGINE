#ifndef _MESH_H_
#define _MESH_H_

#include "../Core.h"
#include "../ResourceTools/Resources.h"

class Mesh {
public:
	std::string meshName;
	Resources::Material* material;
	int vertexOffset; //offset into vao to begin drawing this mesh
	int size; //number of vertices this mesh has

	Mesh(std::string& name, Resources::Material* mtl, int vertOff, int meshSize);
	~Mesh();
	std::string& getMeshName();
	bool setMaterial(Resources::Material*);
	Resources::Material* getMaterial();
};

#endif
