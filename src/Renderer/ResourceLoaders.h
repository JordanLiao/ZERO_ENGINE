#ifndef _RESOURCELOADERS_H_
#define _RESOURCELOADERS_H_

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "../Core.h"
#include "Resources.h"

namespace ResourceLoaders {
	GLuint loadTexture(const char* fName);
	std::unordered_map<std::string, Resources::Material*>* loadMaterialMap(const char* fName);
}

#endif

