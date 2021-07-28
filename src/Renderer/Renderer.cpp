#include "Renderer.h"

Shader* Renderer::shaders[shaderRole::shaderSize];
Shader* Renderer::currShader = NULL; //currently using shader program

Camera* Renderer::cameras[cameraRole::cameraSize];
Camera* Renderer::currCamera;

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
	shaders[shaderRole::colorPickingShader]->bind(currCamera->view, Window::projection);
	shaders[shaderRole::colorPickingShader]->setUniformMat4("model", model);
	setColorCoding(colorCode);

	Window::bindFramebuffer(framebuffer::pickingFrame); //bind the offscreen picking framebuffer to draw on
	glcheck(glBindVertexArray(vao));
	glcheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * offset)));
	Window::bindFramebuffer(framebuffer::defaultFrame); //switch back the default framebuffer 
	shaders[shaderRole::colorPickingShader]->unbind();
}

/*
	take in a material struct, and prepare the shader based on the material properties as follows:
	-determine shader type based on illum model
	-send the uniform values to the shader
*/
bool Renderer::prepShader(Resources::Material* mat) {
	currShader->bind(currCamera->view, Window::projection);
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

/*
	set the color code uniform in the color picking shader
*/
bool Renderer::setColorCoding(int code)  {
	int r = (code & 0x000000FF);
	int g = (code & 0x0000FF00) >> 8;
	int b = (code & 0x00FF0000) >> 16;
	shaders[shaderRole::colorPickingShader]->setUniorm4F("colorCode", r/255.0f, g/255.0f, b/255.0f, 1.0f);
	return true;
}

void Renderer::addShader(shaderRole sRole, Shader* s) {
	shaders[sRole] = s;
}

bool Renderer::setShader(shaderRole sRole) {
	if (shaders[sRole] == NULL) // shader of this role does not exist yet
		return false;
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

Camera* Renderer::getCurrCamera() {
	return currCamera;
}
