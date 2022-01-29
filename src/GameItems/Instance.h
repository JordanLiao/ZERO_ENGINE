#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>

#include "../Core.h"
#include "../tools/ColorID.h"
#include "Object.h"

class Instance {
private:
	glm::mat4 model;
	Object* object; //the geometry that this instance is based on; multiple instances may use the same object
public:
	int colorId;
	glm::vec3 pos;
	std::string instanceName;

	Instance(Object *);
	~Instance();

	void render(glm::mat4);
	void renderColorCode(glm::mat4);
	//transformation functions
	void translate(glm::vec3 trans);
};

#endif // !_INSTANCE_H_
