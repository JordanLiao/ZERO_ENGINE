#ifndef _MESH_H_
#define _MESH_H_

#include "../Core.h"
#include "Resources.h"

class Mesh {
private:
	std::string meshName;
	GLuint vao;
	GLuint vbo[3];
	GLuint ebo;
	std::string materialName;
	Resources::Material* material;

public:

	~Mesh();
	GLuint getVao() const;
	void bind() const;
	void unbind() const;
};

#endif
