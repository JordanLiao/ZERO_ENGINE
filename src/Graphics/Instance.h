#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>

#include "../Core.h"
#include "../tools/ColorID.h"
#include "Object.h"

class Instance {
public:
	glm::mat4 model;
	Object* object; //the graphics object that this instance is based on; multiple instances may use the same object
	int colorId;
	glm::vec3 pos;
	std::string instanceName;

	Instance(Object *);
	~Instance();

	//transformation functions
	void translate(glm::vec3 trans);
};

#endif
