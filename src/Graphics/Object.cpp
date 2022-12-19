#include "Object.h"

Object::Object(std::string& objName, GLuint vertArrObj, glm::mat4 m) {
    objFileName = objName;
    vao = vertArrObj;
    model = m;
}

Object::~Object() {
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


