#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <unordered_map>
#include <string>

#include "Core.h"
#include "tools/ResourceLoaders.h"
#include "Resources.h"

class ResourceManager {
private:
	std::unordered_map<std::string, GLuint> textureMap; //map of texture name to texture id
	std::unordered_map<std::string, Resources::Material*> mtlMap; //map of material name to material struct pointer

public:
	ResourceManager();
	~ResourceManager();
	GLuint getTextureId(std::string& textureName);
	Resources::Material* getMaterial(std::string& materialName);
	bool loadTextures();
	bool loadMaterials();
	void deleteTexture();
	void deleteResource();
};

#endif

