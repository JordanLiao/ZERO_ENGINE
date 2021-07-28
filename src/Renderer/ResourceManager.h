#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Core.h"
#include "Resources.h"
#include "Mesh.h"
#include "Object.h"

class ResourceManager {
private:
	static std::unordered_map<std::string, GLuint> textureMap; //map of texture name to texture id
	//mapping mtl file names to maps of mtl values. Design decision due to the fact that mtl file
	//names are unique, whereas single mtl value may not be.
	static std::unordered_map<std::string, std::unordered_map<std::string, Resources::Material*>*> mtlMapMap;

public:
	static GLuint getTextureId(std::string& textureName);
	static std::unordered_map<std::string, Resources::Material*> getMaterialMap(std::string& materialMapName);
	static bool loadTextures();
	static bool loadMaterials();
	static void deleteTexture();
	static void deleteMaterial();
	static GLuint loadTexture(std::string & fileName);
	static void loadMaterialMap(std::unordered_map<std::string, Resources::Material*>& matMap, const char* fPath);
	static Object* loadObject(const char* fName);
	static std::string getFileNameFromPath(std::string& fPath);
	static std::string getFolderPath(std::string& fPath);
};

#endif

