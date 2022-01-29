#include "Object.h"

Object::Object(std::string& objName, GLuint vertArrObj) {
    objFileName = objName;
    vao = vertArrObj;
    model = glm::mat4(1.f);
    pos = glm::vec3(0.f);
}

Object::Object(std::string& objName, std::string& mtlName, std::vector<Mesh*> & meshes, 
std::unordered_map<std::string, Resources::Material*> & materials, GLuint vertArrObj) {
    objFileName = objName;
    mtlFileName = mtlName;
    meshList = meshes;
    matMap = materials;
    model = glm::mat4(1.f);
    pos = glm::vec3(0.f);
    vao = vertArrObj;
}

Object::~Object() {
}

void Object::render(glm::mat4 m) {
    //std::cout << "rendering " << objFileName << std::endl;
    glm::mat4 combinedModel = m * model;
    for (int i = 0; i < meshList.size(); i++) {
        Renderer::draw(combinedModel, meshList[i]->material, vao, meshList[i]->vertexOffset, meshList[i]->size);
    }
}

void Object::renderColorCode(int colorId, glm::mat4 m) {
    glm::mat4 combinedModel = m * model;
    for (int i = 0; i < meshList.size(); i++) {
        Renderer::drawToColorPickingFrameBuffer(combinedModel, vao, meshList[i]->vertexOffset, meshList[i]->size, colorId);
    }
}

void Object::setMeshList(std::vector<Mesh*>& mList)
{
    meshList = mList;
}

void Object::setMatMap(std::unordered_map<std::string, Resources::Material*>& mMap)
{
    matMap = mMap;
}

void Object::setMtlName(std::string& mtlName)
{
    mtlFileName = mtlName;
}

void Object::setVao(GLuint vertArrObj) {
    vao = vertArrObj;
}

void Object::setEbo(GLuint elementBuffer) {
    ebo = elementBuffer;
}
void Object::setVbo(GLuint* vbos, int size) {
    for (int i = 0; i < size; i++) {
        vbo.push_back(vbos[i]);
    }
}

void Object::translate(glm::vec3 trans){
    pos += trans;
    model = glm::translate(pos);
}

