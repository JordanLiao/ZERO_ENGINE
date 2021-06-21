#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <unordered_map>
#include <string>

#include "../Core.h"
#include "ResourceLoaders.h"
#include "Resources.h"

class ResourceManager {
private:
	std::unordered_map<std::string, GLuint> textureMap; //map of texture name to texture id
	//mapping mtl file names to maps of mtl values. Design decision due to the fact that mtl file
	//names are unique, whereas single mtl value may not be.
	std::unordered_map<std::string, std::unordered_map<std::string, Resources::Material*>*> mtlMapMap;

public:
	ResourceManager();
	~ResourceManager();
	GLuint getTextureId(std::string& textureName);
	std::unordered_map<std::string, Resources::Material*> getMaterialMap(std::string& materialMapName);
	bool loadTextures();
	bool loadMaterials();
	void deleteTexture();
	void deleteMaterial();
};

#endif

