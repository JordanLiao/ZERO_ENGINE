#ifndef _RESOURCELOADERS_H_
#define _RESOURCELOADERS_H_

#include <string>
#include <unordered_map>

#include "Core.h"
#include "Resources.h"

namespace ResourceLoaders {
	static GLuint loadTexture(const char* fName);
	static void loadMaterial(std::unordered_map<std::string, Resources::Material*>&, const char* fName);
}

#endif

