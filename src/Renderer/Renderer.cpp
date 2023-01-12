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

void Renderer::drawInstance(Instance* inst, glm::mat4 m, shaderRole sRole, LightSource * light) {
	Object* instObj = inst->object;
	glm::mat4 combinedModel = m * inst->model * instObj->model;
	Shader* shader = shaders[sRole];
	shader->bind();
	shader->setUniformMat4("model", combinedModel);
	//shader->setUniformVec3("viewPos", currCamera->eyePos);
	//shader->setUniformMat4("projView", Window::projection * currCamera->view);
	setLight(light, sRole);

	//set the uniforms based on the shader type
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
	std::size_t meshListSize = instObj->meshList.size();
	for (std::size_t i = 0; i < meshListSize; i++) {
		Mesh* mesh = instObj->meshList[i];
		if (sRole != shadowMapShader) {
			prepMaterial(mesh->material, shader);
		}
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->indexOffset)));
	}
}

void Renderer::drawAnimatedInstance(Instance* inst, LightSource* light, std::size_t animID) {
	Object* instObj = inst->object;

	Shader* shader = shaders[skeletalAnimationShader];
	shader->bind();
	setLight(light, skeletalAnimationShader);
	shader->setUniformVec3("viewPos", currCamera->eyePos);
	shader->setUniformMat4("projView", Window::projection * currCamera->view);

	//set up the bone transforms
	//SkeletalAnimation* anim = instObj->animations[animID];
	std::vector<glm::mat4> boneTransformation(100, glm::mat4(1.0f));
	glm::mat4 inverseRootModel = glm::inverse(instObj->root->model);
	glm::mat4 identity = glm::mat4(1.0f);
	getBoneTransformation(boneTransformation, instObj->root, inverseRootModel, identity, instObj->boneDataList, instObj->boneNameToID);
	shader->setUniformMat4V("boneTransforms", 100, false, glm::value_ptr(boneTransformation[0]));

	glcheck(glBindVertexArray(instObj->vao));
	std::size_t meshListSize = instObj->meshList.size();
	for (std::size_t i = 0; i < meshListSize; i++) {
		Mesh* mesh = instObj->meshList[i];
		prepMaterial(mesh->material, shader);
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->indexOffset)));
	}
}

void Renderer::getBoneTransformation(std::vector<glm::mat4>& boneTransformation, Node* node, glm::mat4& rootInverseModel,
	glm::mat4& parentModel, std::vector<Bone*>& boneDataList, std::unordered_map<std::string, unsigned int>& boneNameToID) {
	glm::mat4 thisModel = parentModel * node->model;
	if (boneNameToID.find(node->nodeName) != boneNameToID.end()) {
		unsigned int boneID = boneNameToID[node->nodeName];
		boneTransformation[boneID] = rootInverseModel * thisModel * boneDataList[boneID]->offsetMat;
	}

	for (Node* n : node->children) {
		getBoneTransformation(boneTransformation, n, rootInverseModel, thisModel, boneDataList, boneNameToID);
	}
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
	shader->setUniform4F("colorCode", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	glcheck(glBindVertexArray(instObj->vao));
	for (int i = 0; i < instObj->meshList.size(); i++) {
		Mesh* mesh = instObj->meshList[i];
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->indexOffset)));
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
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->indexOffset)));
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
		glcheck(glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * mesh->indexOffset)));
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
	shader->setUniform1F("specHighlight", mat->specularFocus);
	shader->setUniform1F("dissolve", mat->nonTransparency);
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
	if (sRole == phongShader || sRole == phongShadowShader || sRole == skeletalAnimationShader) { // set up material uniforms
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


