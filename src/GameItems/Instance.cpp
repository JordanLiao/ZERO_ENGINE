#include "Instance.h"

Instance::Instance(Object* obj) {
    object = obj;
    model = glm::mat4(1);
    colorId = ColorID::getNewId();
    instanceName = obj->objFileName + std::to_string(colorId);
}

Instance::~Instance() {
}

void Instance::render(glm::mat4 m) {
    object->render(m * model);
}

void Instance::renderColorCode(glm::mat4 m) {
    object->renderColorCode(colorId, m * model);
}

void Instance::translate(glm::vec3 trans) {
    pos += trans;
    model = glm::translate(pos);
}
