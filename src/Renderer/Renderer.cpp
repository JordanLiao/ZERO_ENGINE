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
void Renderer::draw(glm::mat4 model, Resources::Material* mat, GLuint vao, int offset, int count, glm::vec3 eye) {
	//set shader uniforms
	shader->bind();
	shader->setUniformMat4("model", model);
	shader->setUniformVec3("viewPos", eye);
	//material uniforms
	shader->setUniformVec3("ambientColor", mat->ambient);
	shader->setUniformVec3("diffuseFactor",mat->diffuse);
	shader->setUniformVec3("specColor", mat->specular);
	shader->setUniorm1F("specHighlight", mat->specularFocus);
	shader->setUniorm1F("dissolve", mat->nonTransparency);
	glcheck(glBindTexture(GL_TEXTURE_2D, mat->diffuseTexture));
	
	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint)*offset)));

	glcheck(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::setShader(Shader* s) {
	shader = s;
}
