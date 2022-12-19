#include "Mesh.h"

Mesh::Mesh(std::string& name,Resources::Material* mtl, int indOff, int meshSize) {
    meshName = name;
    material = mtl;
    indexOffset = indOff;
    size = meshSize;
}

Mesh::~Mesh() {
    delete material;
}

std::string& Mesh::getMeshName() {
    return meshName;
}

bool Mesh::setMaterial(Resources::Material*) {
    return false;
}

Resources::Material* Mesh::getMaterial() {
    return nullptr;
}
