#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Core.h"
#include "Shader.h"
#include "Resources.h"

class Renderer {
private:
	Shader* shader;
public:
	Renderer();
	~Renderer();
	void draw(Resources::Material* mat, GLuint vao, int offset, int count);
	void setShader(Shader * s);
};

#endif
