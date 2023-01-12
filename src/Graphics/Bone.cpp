#include "Bone.h"

Bone::Bone(glm::mat4 bindingPoseOffset) {
	this->offsetMat = bindingPoseOffset;
	model = glm::mat4(1.f);
}
