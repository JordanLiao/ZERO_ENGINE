#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Core.h"
#include "Shader.h"

class Renderer {
private:

public:
	void draw(const GLuint vao, const Shader* shader) const;
};

#endif
