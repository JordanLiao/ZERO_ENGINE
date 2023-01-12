#include "Object.h"

Object::Object(std::string& objName, GLuint vertArrObj, glm::mat4 m) {
    objFileName = objName;
    vao = vertArrObj;
    model = m;
    root = new Node();
    boneDataList.push_back(new Bone(glm::mat4(1.f)));
}

Object::~Object() {
    size_t size = boneDataList.size();
    for (size_t i = 0; i < size; i++)
        delete boneDataList[i];

    size = animations.size();
    for (size_t i = 0; i < size; i++)
        delete animations[i];

    size = meshList.size();
    for (size_t i = 0; i < size; i++)
        delete meshList[i];

    delete root;
}

void Object::setMatMap(std::unordered_map<std::string, Resources::Material*>& mMap) {
    matMap = mMap;
}

void Object::setVao(GLuint vertArrObj) {
    vao = vertArrObj;
}



