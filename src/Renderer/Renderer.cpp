#include "Renderer.h"

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

/*
	mat: material of the mesh 
	vao: vertex array object to be used
	offset: offset into the vao to start drawing
	count: number of vertices to be drawn
*/
void Renderer::draw(Resources::Material* mat, GLuint vao, int offset, int count) {
	//set shader uniforms
	shader->bind();

	glcheck(glBindVertexArray(vao));
}

void Renderer::setShader(Shader* s) {
	shader = s;
}
