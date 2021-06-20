#include "Renderer.h"

void Renderer::draw(const GLuint vao, const Shader* shader) const
{
	shader->bind();
	//set shader uniforms

	glcheck(glBindVertexArray(vao));
}
