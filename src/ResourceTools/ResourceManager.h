#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <unordered_map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Core.h"
#include "Resources.h"
#include "../GameItems/Mesh.h"
#include "../GameItems/Instance.h"
//#include "../GameItems/Scene.h"

class ResourceManager {
private:
	//map of texture name to texture id
	static std::unordered_map<std::string, GLuint> textureMap;
	//mapping mtl "file" names to maps of mtl values. Design decision due to the fact that mtl file
	//names are unique, whereas single mtl struct may not be.
	static std::unordered_map<std::string, std::unordered_map<std::string, Resources::Material*>*> mtlMapMap;
	//static Resources::Material defaultMat;

public:
	//mapping object names to objects
	static std::unordered_map<std::string, Object*> objMap;
	static std::list<Object*> objList;

	static void init();
	static GLuint getTextureId(std::string& textureName);
	static std::unordered_map<std::string, Resources::Material*> getMaterialMap(std::string& materialMapName);
	static bool loadTextures();
	static bool loadMaterials();
	static void deleteTexture();
	static void deleteMaterial();
	static GLuint loadTexture(std::string & fileName);
	static bool loadMaterialMap(std::unordered_map<std::string, Resources::Material*>& matMap, const char* fPath);
	static Object* loadObject(const char* fName);
	static std::string getFileNameFromPath(std::string& fPath);
	static std::string getFolderPath(std::string& fPath);
};

#endif

