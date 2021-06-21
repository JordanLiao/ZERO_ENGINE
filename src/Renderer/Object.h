#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>

#include "../Core.h"
#include "Mesh.h"
#include "ResourceLoaders.h"

class Object {
private:
	std::vector<Mesh> meshList; //might want to use a different data structure for efficiency

public:
	Object(const char* objName);
	~Object();
	void render();
};

#endif

