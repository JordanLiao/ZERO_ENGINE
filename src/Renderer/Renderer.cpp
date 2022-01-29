#include "Renderer.h"

Shader* Renderer::shaders[shaderRole::shaderSize];
Shader* Renderer::currShader = NULL; //currently using shader program

Camera* Renderer::cameras[cameraRole::cameraSize];
Camera* Renderer::currCamera;
LightSource* Renderer::currLight;

void Renderer::initRenderer(Camera * cam) {
	if (cam == NULL) {
		cameras[cameraRole::engineDisplayCam] = new Camera();
		currCamera = cameras[cameraRole::engineDisplayCam];
	}
}

/*
	mat: material of the mesh 
	vao: vertex array object to be used
	offset: offset into the vao to start drawing
	count: number of vertices to be drawn
*/
void Renderer::draw(glm::mat4 model, Resources::Material* mat, GLuint vao, int offset, int count) {
	prepShader(mat);
	currShader->setUniformMat4("model", model);
	currShader->setUniformVec3("viewPos", currCamera->eyePos);
	
	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint)*offset)));

	glcheck(glBindTexture(GL_TEXTURE_2D, 0));
	currShader->unbind();
}

/*
	render color coded objects to an offscreen framebuffer for color picking
*/

//note there is a bug that "bound" variable in shader is not reliable if there are more than one shader in the program
void Renderer::drawToColorPickingFrameBuffer(glm::mat4 model, GLuint vao, int offset, int count, int colorCode) {
	Shader* shader = shaders[shaderRole::colorPickingShader];
	shader->bind();
	shader->setUniformMat4("model", model);
	shader->setUniformMat4("view", currCamera->view);
	shader->setUniformMat4("projection", Window::projection);
	
	//calculate rgb values from color code
	int r = (colorCode & 0x000000FF);
	int g = (colorCode & 0x0000FF00) >> 8;
	int b = (colorCode & 0x00FF0000) >> 16;
	shader->setUniorm4F("colorCode", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * offset)));
	shader->unbind();
}

void Renderer::drawToShadowMapFrameBuffer(glm::mat4 model, GLuint vao, int offset, int count) {
	Shader* shadowMapS = shaders[shadowMapShader];
	shadowMapS->bind();
	shadowMapS->setUniformMat4("projView", currLight->projView);
	shadowMapS->setUniformMat4("model", model);
}

/*
	take in a material struct, and prepare the main render shader based on the material properties as follows:
	-determine shader type based on illum model
	-send the uniform values to the shader
*/
bool Renderer::prepShader(Resources::Material* mat) {
	currShader->bind();
	if (mat == nullptr) { // if there isn't any material, use default material
		return true;
	}
	currShader->setUniformMat4("view", currCamera->view);
	currShader->setUniformMat4("projection", Window::projection);
	//set up uniforms based on the shader being used
	if (currShader == shaders[shaderRole::phongShader]) { // set up material uniforms
		currShader->setUniformVec3("ambientColor", mat->ambient);
		currShader->setUniformVec3("diffuseFactor", mat->diffuse);
		currShader->setUniformVec3("specColor", mat->specular);
		currShader->setUniorm1F("specHighlight", mat->specularFocus);
		currShader->setUniorm1F("dissolve", mat->nonTransparency);
		glcheck(glBindTexture(GL_TEXTURE_2D, mat->diffuseTexture));
	}
	return true;
}

void Renderer::addShader(shaderRole sRole, Shader* s) {
	shaders[sRole] = s;
}

bool Renderer::setShader(shaderRole sRole) {
	if (shaders[sRole] == NULL) { // shader of this role does not exist yet
		std::cout << "Shader " << sRole << " is not loaded." << std::endl;
		return false;
	}
	//shaders[sRole]->bind();
	currShader = shaders[sRole];
	return true;
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

void Renderer::setLight(LightSource* light) {
	if (light == nullptr)
		return;
	if (currShader == shaders[shaderRole::phongShader]) { // set up material uniforms
		currShader->bind();
		currShader->setUniformVec3("directionalLightDir", light->direction);
		currShader->setUniformVec3("pointLightPos", light->position);
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
