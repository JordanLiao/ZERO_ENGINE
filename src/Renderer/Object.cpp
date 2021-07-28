#include "Object.h"

Object::Object(std::string& objName, GLuint vertArrObj) {
    objFileName = objName;
    vao = vertArrObj;
    model = glm::mat4(1.f);
    pos = glm::vec3(0.f);
    setColorId();
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
    setColorId();
}

Object::~Object() {
}

/*
    set up a unique color id for this object for color picking
*/
void Object::setColorId() {
    colorId = ColorID::getNewId();
}

void Object::render() {
    for (int i = 0; i < meshList.size(); i++) {
        Renderer::draw(model, meshList[i]->material, vao, meshList[i]->vertexOffset, meshList[i]->size);
    }
}

void Object::renderColorCode() {
    for (int i = 0; i < meshList.size(); i++) {
        Renderer::drawToColorPickingFrameBuffer(model, vao, meshList[i]->vertexOffset, meshList[i]->size, colorId);
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

