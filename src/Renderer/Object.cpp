#include "Object.h"

Object::Object(std::string& objName, GLuint vertArrObj) {
    objFileName = objName;
    vao = vertArrObj;
}

Object::Object(std::string& objName, std::string& mtlName, std::vector<Mesh*> & meshes, 
std::unordered_map<std::string, Resources::Material*> & materials, GLuint vertArrObj) {
    objFileName = objName;
    mtlFileName = mtlName;
    meshList = meshes;
    matMap = materials;
}

Object::~Object()
{
}


void Object::render() {
    for (int i = 0; i < meshList.size(); i++) {
        
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
