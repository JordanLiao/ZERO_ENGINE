#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"

std::unordered_map<std::string, GLuint> ResourceManager::textureMap; //map of texture name to texture id
	//mapping mtl file names to maps of mtl values. Design decision due to the fact that mtl file
	//names are unique, whereas single mtl value may not be.
std::unordered_map<std::string, std::unordered_map<std::string, Resources::Material*>*> ResourceManager::mtlMapMap;
//mapping object names to objects
std::unordered_map<std::string, Object*> ResourceManager::objMap;
//list to keep track of all the loaded objects
std::list<Object*> ResourceManager::objList;
//Resources::Material ResourceManager::defaultMat;

void ResourceManager::init() {
}

GLuint ResourceManager::getTextureId(std::string& textureName)
{
    return GLuint();
}

std::unordered_map<std::string, Resources::Material*> ResourceManager::getMaterialMap(std::string& materialMapName)
{
    return std::unordered_map<std::string, Resources::Material*>();
}

bool ResourceManager::loadTextures()
{
    return false;
}

bool ResourceManager::loadMaterials()
{
    return false;
}

void ResourceManager::deleteTexture()
{
}

void ResourceManager::deleteMaterial()
{
}

GLuint ResourceManager::loadTexture(std::string & fPath) {
	std::string textureName = getFileNameFromPath(fPath);
	//if texture is already loaded
	if (textureMap.find(textureName) != textureMap.end())
		return textureMap[textureName];

	int ftw, fth, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fPath.c_str(), &ftw, &fth, &channels, STBI_rgb_alpha);
	if (data == NULL) {
		std::cout << "cannot load texture at " << fPath << std::endl;
		return 0;
	}
	GLuint textId;
	glcheck(glGenTextures(1, &textId));
	glcheck(glBindTexture(GL_TEXTURE_2D, textId));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ftw, fth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	glcheck(glGenerateMipmap(GL_TEXTURE_2D));
	stbi_image_free(data);

	//store texture id in textureMap for possible later use
	textureMap[textureName] = textId;

	return textId;
}

/*
	load a .mtl file as a material map, and store this map in a map of material maps for future reference
*/
bool ResourceManager::loadMaterialMap(std::unordered_map<std::string, Resources::Material*>& matMap,
	const char* fPath) {

	std::ifstream objFile(fPath);
	if (!objFile.is_open()) {
		std::cerr << "Can't open mtl file: " << fPath << std::endl;
		return false;
	}

	//provide a default material
	Resources::Material* curr = NULL;

	std::string line;
	while (std::getline(objFile, line)) {
		std::stringstream ss;
		ss << line;
		std::string label;
		ss >> label;
		if (label == "#")
			continue;

		else if (label == "newmtl") {
			if (curr != NULL) { //insert the previous material if it is not NULL
				//std::cout << "putting " << curr->materialName << std::endl;
				matMap[curr->materialName] = curr;
			}
			curr = new Resources::Material();
			ss >> curr->materialName;
			//std::cout << "loading " << curr->materialName << std::endl;
		}

		else if (curr == NULL)
			continue;

		else if (label == "Ka") {
			ss >> curr->ambient.x >> curr->ambient.y >> curr->ambient.z;
		}

		else if (label == "Kd") {
			ss >> curr->diffuse.x >> curr->diffuse.y >> curr->diffuse.z;
		}

		else if (label == "Ks") {
			ss >> curr->specular.x >> curr->specular.y >> curr->specular.z;
		}

		else if (label == "Ns") {
			ss >> curr->specularFocus;
		}

		else if (label == "Ni") {
			ss >> curr->opticalDensity;
		}

		else if (label == "d") {
			ss >> curr->nonTransparency;
		}

		else if (label == "illum") {
			ss >> curr->illum;
		}

		else if (label == "map_Kd") {  //diffuse texture map
			std::string textureName;
			ss >> textureName;
			if (textureMap.find(textureName) != textureMap.end())
				curr->diffuseTexture = textureMap[textureName];
			else {
				std::string texturePath = std::string("Assets/") + textureName;
				curr->diffuseTexture = loadTexture(texturePath);
			}
		}
	}
	if (curr != NULL) //insert the previous material if it is not NULL
	{
		//std::cout << "putting " << curr->materialName << std::endl;
		matMap[curr->materialName] = curr;
	}

	objFile.close();
	return true;
}

Object* ResourceManager::loadObject(const char* fPath) {
	std::cout << "loading " << fPath << std::endl;
	std::ifstream objFile(fPath);
	if (!objFile.is_open()) {
		std::cerr << "Can't open obj file: " << fPath << std::endl;
		return NULL;
	}

	std::string objPath = std::string(fPath);
	std::string objName = getFileNameFromPath(objPath);
	std::string folderPath = getFolderPath(objPath);

	//if obj is already loaded, skip the rest
	if (objMap.find(objName) != objMap.end()) {
		std::cout << objName << " is already loaded." << std::endl;
		return objMap[objName];
	}

	//if obj is not loaded
	GLuint vao; //this vao will be shared with all meshes
	glcheck(glGenVertexArrays(1, &vao));
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Resources::Material*> matMap;

	//want to have these temporaries of vertex data becasue their sizes might not be the same
	//so there might be a need to re-arrange them.
	std::vector<glm::vec3> normalsp;
	std::vector<glm::vec3> pointsp;
	std::vector<glm::vec2> texp;
	//final vertex data
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> points;
	std::vector<glm::vec2> textCoords;
	std::vector<glm::ivec3> triangles;

	bool hasMaterial = false;

	//--all the temporary values needed to parse
	bool loadingMesh = false; // whether parser has began loading meshes; initially set to false
	std::string delim = "/";
	//--counting the number of vertex in face section because face values need
	//to be the same like 1/1/1 instead of like 2/4/1.
	int vCount = 0;
	int currMeshTriangleCount = 0;
	size_t aPos, bPos, cPos, a1Pos, b1Pos, c1Pos;
	Resources::Material* currMeshMtl = new Resources::Material();
	std::stringstream ss;
	std::string line, label, mtlName, currMeshMtlName, a, b, c, currMeshName;
	glm::vec3 point;
	glm::vec2 tex;
	glm::vec3 norm;
	glm::ivec3 triangle;

	while (std::getline(objFile, line)) {
		ss.clear(); //clear string stream first
		ss.str(line);
		//ss << line;
		ss >> label;
		if (label == "#") { //obj file comments
			continue;
		}

		//obj mtl name
		else if (label == "mtllib") {
			ss >> mtlName;
			std::cout << "loading " << mtlName << std::endl;
			if (loadMaterialMap(matMap, (folderPath + mtlName).c_str()))
				hasMaterial = true;
		}

		else if (label == "v") {
			ss >> point.x >> point.y >> point.z;
			pointsp.push_back(point);
		}

		else if (label == "vt")
		{
			ss >> tex.x >> tex.y;
			texp.push_back(tex);
		}

		//get all the vertex normals
		else if (label == "vn") {
			ss >> norm.x >> norm.y >> norm.z;
			norm = glm::normalize(norm);
			normalsp.push_back(norm);
		}

		//mesh material name
		else if (label == "usemtl") {
			ss >> currMeshMtlName;
			if (matMap.find(currMeshMtlName) != matMap.end()) {//if found within material map
				delete currMeshMtl; //free default mtl
				currMeshMtl = matMap[currMeshMtlName];
			}
		}

		else if (label == "f") {
			ss >> a >> b >> c;

			//find the locations of the two delimiters
			aPos = a.find(delim);
			bPos = b.find(delim);
			cPos = c.find(delim);
			a1Pos = a.rfind(delim);
			b1Pos = b.rfind(delim);
			c1Pos = c.rfind(delim);

			//get vertex points
			points.push_back(pointsp[std::stoi(a.substr(0, aPos)) - 1]);
			points.push_back(pointsp[std::stoi(b.substr(0, bPos)) - 1]);
			points.push_back(pointsp[std::stoi(c.substr(0, cPos)) - 1]);

			//get texture coords; use conditions because texture value in f section might not exist like 1//3.
			if (aPos + 1 < a1Pos)
				textCoords.push_back(texp[stoi(a.substr(aPos + 1, a1Pos - aPos - 1)) - 1]);
			if (bPos + 1 < b1Pos)
				textCoords.push_back(texp[stoi(b.substr(bPos + 1, b1Pos - bPos - 1)) - 1]);
			if (cPos + 1 < c1Pos)
				textCoords.push_back(texp[stoi(c.substr(cPos + 1, c1Pos - cPos - 1)) - 1]);

			normals.push_back(normalsp[stoi(a.substr(a1Pos + 1, std::string::npos)) - 1]);
			normals.push_back(normalsp[stoi(b.substr(b1Pos + 1, std::string::npos)) - 1]);
			normals.push_back(normalsp[stoi(c.substr(c1Pos + 1, std::string::npos)) - 1]);

			triangle.x = vCount++;
			triangle.y = vCount++;
			triangle.z = vCount++;
			triangles.push_back(triangle);
			currMeshTriangleCount++;
		}

		else if (label == "o") {
			//if there was a previous mesh
			if (loadingMesh) {
				Mesh* mesh = new Mesh(currMeshName, currMeshMtl,
					vCount - 3 * currMeshTriangleCount, 3 * currMeshTriangleCount);
				meshes.push_back(mesh);
			}
			loadingMesh = true;
			//reset mesh temporaries
			currMeshMtl = NULL;
			currMeshTriangleCount = 0;
			ss >> currMeshName;
		}
	}
	objFile.close();
	// load the last mesh
	Mesh* mesh = new Mesh(currMeshName, currMeshMtl,
		vCount - 3 * currMeshTriangleCount, 3 * currMeshTriangleCount);
	meshes.push_back(mesh);

	//setting up the openGL buffers
	GLuint vbo[3];
	GLuint ebo;

	glcheck(glBindVertexArray(vao));
	glcheck(glGenBuffers(3, vbo));

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(0));
	glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(1));
	glcheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[2]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textCoords.size(), textCoords.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(2));
	glcheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glGenBuffers(1, &ebo));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW));

	// Unbind the VBO/VAO
	glcheck(glBindVertexArray(0));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	Object* obj = new Object(objName, mtlName, meshes, matMap, vao);
	objMap[objName] = obj;
	objList.push_back(obj);

	return obj;
}

/*
    extract the file name from the file path
*/
std::string ResourceManager::getFileNameFromPath(std::string& fPath) {
    return fPath.substr(fPath.rfind("/") + 1, std::string::npos);
}

/*
    extract the folder path /a/b/ from file path /a/b/file*
*/
std::string ResourceManager::getFolderPath(std::string& fPath) {
    return fPath.substr(0, fPath.rfind("/") + 1);
}
