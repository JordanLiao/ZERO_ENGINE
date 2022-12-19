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

bool ResourceManager::loadTexture()
{
    return false;
}

Resources::Material * ResourceManager::loadMaterial(const aiMaterial * mtl) {
	Resources::Material * mat = new Resources::Material();
	mat->materialName = std::string(mtl->GetName().C_Str());

	aiColor4D color;
	float val;
	int intVal;
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color)) {
		mat->ambient = glm::vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color)) {
		mat->diffuse = glm::vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color)) {
		mat->specular = glm::vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color)) {
		mat->emissive = glm::vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_SHININESS, &val)) {
		mat->specularFocus = val;
	}

	if (AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_OPACITY, &val)) {
		mat->nonTransparency = val;
	}

	if (AI_SUCCESS == aiGetMaterialInteger(mtl, AI_MATKEY_SHADING_MODEL, &intVal)) {
		mat->illum = intVal;
	}

	return mat;
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

Object* ResourceManager::loadObject(const char* fPath) {
	std::string pathName = std::string(fPath);
	std::string objName = std::string(getFileNameFromPath(pathName));
	
	// if this object was loaded previously
	if (objMap.find(objName) != objMap.end())
		return objMap[objName];

	Assimp::Importer imp;
	//const aiScene* pScene = imp.ReadFile(fPath, aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_FlipWindingOrder | aiProcess_FixInfacingNormals );
	const aiScene* pScene = imp.ReadFile(fPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals);

	if (pScene == nullptr) {
		printf("Error parsing '%s': '%s'\n", fPath, imp.GetErrorString());
		return nullptr;
	}
	
	Object* obj = new Object(objName);
	// Initialize the meshes in the obj file one by one
	std::vector<glm::vec3> vert, norm;
	std::vector<glm::vec2> text;
	std::vector<glm::ivec3> indices;
	int vertexOffset = 0; //denotes the boundary in "vert" where a mesh begins
	int indexOffset = 0; //denotes the boundary in "indices" where a mesh begins
	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		aiMesh* pMesh = pScene->mMeshes[i];
		aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		for (unsigned int j = 0; j < pMesh->mNumVertices; j++) {
			vert.push_back(glm::vec3(pMesh->mVertices[j].x, pMesh->mVertices[j].y, pMesh->mVertices[j].z));
			//because "aiProcess_GenSmoothNormals" flag above we can always explect normals
			norm.push_back(glm::vec3(pMesh->mNormals[j].x, pMesh->mNormals[j].y, pMesh->mNormals[j].z)); 
			aiVector3D textCoord = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][j] : Zero3D;
			text.push_back(glm::vec2(textCoord.x, textCoord.y));
		}

		for (unsigned int j = 0; j < pMesh->mNumFaces; j++) {
			aiFace face = pMesh->mFaces[j];
			//need to offset the index by the number of vertices in the previous meshes because the indices for a mesh
			//are local to a mesh, so they all start from 0.
			indices.push_back(glm::ivec3(face.mIndices[0] + vertexOffset, face.mIndices[1] + vertexOffset, face.mIndices[2] + vertexOffset));
		}

		aiMaterial* mtl = pScene->mMaterials[pMesh->mMaterialIndex];
		std::string meshName = std::string(pMesh->mName.C_Str());
		Mesh* mesh = new Mesh(meshName, loadMaterial(mtl), indexOffset, pMesh->mNumFaces * 3);
		obj->meshList.push_back(mesh);
		indexOffset = indexOffset + pMesh->mNumFaces * 3;
		vertexOffset = vertexOffset + pMesh->mNumVertices;
	}

	//send the data to the gpu
	GLuint vao;
	GLuint vbo[3];
	GLuint ebo;
	glcheck(glGenVertexArrays(1, &vao));
	glcheck(glBindVertexArray(vao));
	
	glcheck(glGenBuffers(3, vbo));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vert.size(), vert.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(0));
	glcheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[1]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norm.size(), norm.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(1));
	glcheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glBindBuffer(GL_ARRAY_BUFFER, vbo[2]));
	glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * text.size(), text.data(), GL_STATIC_DRAW));
	glcheck(glEnableVertexAttribArray(2));
	glcheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0));

	glcheck(glGenBuffers(1, &ebo));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW));

	// Unbind the VBO/VAO
	glcheck(glBindVertexArray(0));

	obj->vao = vao; //shared vao variable
	objMap[objName] = obj;
	objList.push_back(obj);

	return obj;
}

Node* ResourceManager::processNode(aiNode* node, std::vector<Mesh*>& meshes) {
	Node* res = new Node();
	getMat4FromAiMatrix(res->model, node->mTransformation);
	//get the meshes belonging to this node
	for(auto i = 0; i < node->mNumMeshes; i++) {
		res->meshList.push_back(meshes[node->mMeshes[i]]);
	}

	//process this node's child nodes
	for (auto i = 0; i < node->mNumChildren; i++) {
		res->children.push_back(processNode(node->mChildren[i], meshes));
	}

	return res;
}

void ResourceManager::getMat4FromAiMatrix(glm::mat4& mat, aiMatrix4x4& aiMat) {
	mat[0][0] = aiMat[0][0]; mat[0][1] = aiMat[0][1]; mat[0][2] = aiMat[0][2]; mat[0][3] = aiMat[0][3];
	mat[1][0] = aiMat[1][0]; mat[1][1] = aiMat[1][1]; mat[1][2] = aiMat[1][2]; mat[1][3] = aiMat[1][3];
	mat[2][0] = aiMat[2][0]; mat[2][1] = aiMat[2][1]; mat[2][2] = aiMat[2][2]; mat[2][3] = aiMat[2][3];
	mat[3][0] = aiMat[3][0]; mat[3][1] = aiMat[3][1]; mat[3][2] = aiMat[3][2]; mat[3][3] = aiMat[3][3];
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

