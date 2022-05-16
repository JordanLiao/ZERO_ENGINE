/*
NOTE for myself:
	if openGL gives 501 error, first check if your shader compiled or not
*/

#include "Renderer.h"

Shader* Renderer::shaders[shaderRole::shaderSize];

Camera* Renderer::cameras[cameraRole::cameraSize];
Camera* Renderer::currCamera;
LightSource* Renderer::currLight;

InspectionPlane * Renderer::inspectionPlane;

void Renderer::initRenderer(Camera * cam) {
	if (cam == NULL) {
		cameras[cameraRole::engineDisplayCam] = new Camera();
		currCamera = cameras[cameraRole::engineDisplayCam];
	}
	inspectionPlane = new InspectionPlane();
}

/*
	mat: material of the mesh 
	vao: vertex array object to be used
	offset: offset into the vao to start drawing
	count: number of vertices to be drawn
*/
void Renderer::draw(glm::mat4 model, Resources::Material* mat, GLuint vao, int offset, int count, shaderRole sRole) {
	Shader* s = shaders[sRole];
	s->bind();
	prepMaterial(mat, s);
	s->setUniformMat4("model", model);
	s->setUniformVec3("viewPos", currCamera->eyePos);
	s->setUniformMat4("projView", Window::projection * currCamera->view);
	
	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint)*offset)));

	//glcheck(glBindTexture(GL_TEXTURE_2D, 0));
	//currShader->unbind();
}

void Renderer::drawInstance(Instance* inst, glm::mat4 m, shaderRole sRole, LightSource * light) {
	Object* instObj = inst->object;
	glm::mat4 combinedModel = m * inst->model * instObj->model;
	Shader* shader = shaders[sRole];
	shader->bind();
	shader->setUniformMat4("model", combinedModel);
	//shader->setUniformVec3("viewPos", currCamera->eyePos);
	//shader->setUniformMat4("projView", Window::projection * currCamera->view);
	
	setLight(light, sRole);

	//if drawing with shadow
	if (sRole == phongShadowShader) { 
		if (light == nullptr) {
			std::cerr << "phong shadow shader requires a light source." << std::endl;
			return;
		}
		shader->setUniformMat4("lightSpaceMatrix", light->projView);
		glcheck(glActiveTexture(GL_TEXTURE0));
		glcheck(glBindTexture(GL_TEXTURE_2D, Window::shadowMapDepthTexture));
		shader->setUniformVec3("viewPos", currCamera->eyePos);
		shader->setUniformMat4("projView", Window::projection * currCamera->view);
	}
	else if (sRole == shadowMapShader) {
		shader->setUniformMat4("projView", light->projView);
	}
	else {
		shader->setUniformVec3("viewPos", currCamera->eyePos);
		shader->setUniformMat4("projView", Window::projection * currCamera->view);
	}

	glcheck(glBindVertexArray(instObj->vao));
	for (int i = 0; i < instObj->meshList.size(); i++) {
		Mesh* mesh = instObj->meshList[i];
		if (sRole != shadowMapShader) {
			prepMaterial(mesh->material, shader);
		}
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->vertexOffset)));
	}
}

/*
	render color coded objects to an offscreen framebuffer for color picking
*/
void Renderer::drawToColorPickingFrameBuffer(glm::mat4 model, GLuint vao, int offset, int count, int colorCode) {
	Shader* shader = shaders[shaderRole::colorPickingShader];
	shader->bind();
	shader->setUniformMat4("model", model);
	shader->setUniformMat4("projView", Window::projection * currCamera->view);
	
	//calculate rgb values from color code
	int r = (colorCode & 0x000000FF);
	int g = (colorCode & 0x0000FF00) >> 8;
	int b = (colorCode & 0x00FF0000) >> 16;
	shader->setUniorm4F("colorCode", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * offset)));
	//shader->unbind();
}

void Renderer::drawInstanceToColorPickingFrameBuffer(Instance * inst, glm::mat4 m, int colorCode) {
	Object* instObj = inst->object;
	Shader* shader = shaders[shaderRole::colorPickingShader];
	shader->bind();
	shader->setUniformMat4("model", m * inst->model * instObj->model);
	shader->setUniformMat4("projView", Window::projection * currCamera->view);

	//calculate rgb values from color code
	int r = (colorCode & 0x000000FF);
	int g = (colorCode & 0x0000FF00) >> 8;
	int b = (colorCode & 0x00FF0000) >> 16;
	shader->setUniorm4F("colorCode", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	glcheck(glBindVertexArray(instObj->vao));
	for (int i = 0; i < instObj->meshList.size(); i++) {
		Mesh* mesh = instObj->meshList[i];
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->vertexOffset)));
	}

	//shader->unbind();
}

void Renderer::drawInstanceToShadowMapFrameBuffer(Instance* inst, glm::mat4 model, LightSource * light) {
	Object* instObj = inst->object;
	Shader* shader = shaders[shadowMapShader];
	shader->bind();
	shader->setUniformMat4("projView", light->projView);
	shader->setUniformMat4("model", model);

	glcheck(glBindVertexArray(instObj->vao));
	for (int i = 0; i < instObj->meshList.size(); i++) {
		Mesh* mesh = instObj->meshList[i];
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->vertexOffset)));
	}
}

void Renderer::drawShadowInspection() {
	Shader* shader = shaders[shadowInspectionShader];
	shader->bind();
	shader->setUniformMat4("model", glm::mat4(1.0f));
	shader->setUniformMat4("projView", inspectionPlane->projView);

	glcheck(glActiveTexture(GL_TEXTURE0));
	glcheck(glBindTexture(GL_TEXTURE_2D, Window::shadowMapDepthTexture));
	
	Object * instObj = inspectionPlane->obj;
	glcheck(glBindVertexArray(instObj->vao));
	for (int i = 0; i < instObj->meshList.size(); i++) {
		Mesh* mesh = instObj->meshList[i];
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->vertexOffset)));
	}
}

/*
	take in a material struct, and and shader, and pass the material fields as uniform
*/
bool Renderer::prepMaterial(Resources::Material* mat, Shader * shader) {
	//currShader->bind();
	if (mat == nullptr) { // if there isn't any material
		return false;
	}

	shader->setUniformVec3("ambientColor", mat->ambient);
	shader->setUniformVec3("diffuseFactor", mat->diffuse);
	shader->setUniformVec3("specColor", mat->specular);
	shader->setUniorm1F("specHighlight", mat->specularFocus);
	shader->setUniorm1F("dissolve", mat->nonTransparency);
	//if(mat->diffuseTexture != 0)
		//glcheck(glBindTexture(GL_TEXTURE_2D, mat->diffuseTexture));
	return true;
}

void Renderer::addShader(shaderRole sRole, Shader* s) {
	shaders[sRole] = s;
}

void Renderer::addCamera(cameraRole cRole, Camera * cam) {
	cameras[cRole] = cam;
}

bool Renderer::setCamera(cameraRole cRole) {
	if (cameras[cRole] == NULL)
		return false;
	currCamera = cameras[cRole];
	return true;
}

void Renderer::setLight(LightSource* light, shaderRole sRole) {
	Shader* s = shaders[sRole];
	if (light == nullptr)
		return;
	if (sRole == phongShader || sRole == phongShadowShader) { // set up material uniforms
		s->bind();
		s->setUniformVec3("directionalLightDir", light->direction);
		s->setUniformVec3("pointLightPos", light->position);
		s->setUniformVec3("lightColor", light->color);
	}
}

void Renderer::useShadowMap(bool t) {
	if (t)
		Window::bindFramebuffer(framebuffer::shadowMapFrame);
	else
		Window::bindFramebuffer(framebuffer::defaultFrame);
}

Camera* Renderer::getCurrCamera() {
	return currCamera;
}
