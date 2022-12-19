#ifndef _INSPECTIONPLANE_H_
#define _INSPECTIONPLANE_H_

#include "../Core.h"
#include "Instance.h"
#include "../ResourceManagement/ResourceManager.h"

class InspectionPlane {
public:
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 projView;
	glm::mat4 model;
	Object* obj;
	Instance* inst;
	InspectionPlane();
};

#endif