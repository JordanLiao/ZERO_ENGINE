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

BonesForVertex::BonesForVertex(){
	for (size_t i = 0; i < MAX_NUM_BONE_PER_VERTEX; i++) {
		boneIDs[i] = 0;
		boneWeights[i] = 0.f;
	}
}


void BonesForVertex::addBoneWeight(unsigned int boneID, float boneWeight) {
	for (auto i = 0; i < MAX_NUM_BONE_PER_VERTEX; i++) {
		if (boneIDs[i] == boneID)
			return;
		if(boneWeights[i] == 0.f) {
			boneIDs[i] = boneID;
			boneWeights[i] = boneWeight;
			return;
		}
	}
	std::cerr << "ERROR: Vertex has more than " << MAX_NUM_BONE_PER_VERTEX << " of bones." << std::endl;
}

void ResourceManager::init() {
}

GLuint ResourceManager::getTextureId(std::string& textureName){
    return GLuint();
}

std::unordered_map<std::string, Resources::Material*> ResourceManager::getMaterialMap(std::string& materialMapName){
    return std::unordered_map<std::string, Resources::Material*>();
}

bool ResourceManager::loadTexture(){
    return false;
}

Resources::Material * ResourceManager::loadMaterial(const aiMaterial * mtl) {
	Resources::Material * mat = new Resources::Material();
	mat->materialName = std::string(mtl->GetName().C_Str());

	aiColor4D color;
	float val;
	int intVal;
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color))
		mat->ambient = glm::vec3(color.r, color.g, color.b);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color))
		mat->diffuse = glm::vec3(color.r, color.g, color.b);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color))
		mat->specular = glm::vec3(color.r, color.g, color.b);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color))
		mat->emissive = glm::vec3(color.r, color.g, color.b);

	if (AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_SHININESS, &val))
		mat->specularFocus = val;

	if (AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_OPACITY, &val))
		mat->nonTransparency = val;

	if (AI_SUCCESS == aiGetMaterialInteger(mtl, AI_MATKEY_SHADING_MODEL, &intVal))
		mat->illum = intVal;

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
	std::string pathName = std::string(fPath), objName = std::string(getFileNameFromPath(pathName));
	// if this object was loaded previously
	if (objMap.find(objName) != objMap.end())
		return objMap[objName];

	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(fPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals);
	if (pScene == nullptr) {
		printf("Error parsing '%s': '%s'\n", fPath, imp.GetErrorString());
		return nullptr;
	}
	//fixOrientation(pScene);

	Object* obj = new Object(objName);

	std::vector<glm::vec3> vert, norm; //vertices and normal values of meshes
	std::vector<glm::vec2> text; //texture coordinates of meshes
	std::vector<glm::ivec3> indices; //triangle indices of meshes
	std::vector<BonesForVertex> vertToBone; //bone id and weights for each vertex
	//std::unordered_map<std::string, unsigned int> boneNameToID; //this map need to be saved in the corresponding animation object
	//and also an array of inverse bind pose matrices

	int vertexOffset = 0; //denotes the boundary in "vert" where a mesh begins
	int indexOffset = 0; //denotes the boundary in "indices" where a mesh begins

	//Parse the mesh data of the model/scene, including vertex data, bone data etc.
	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		aiMesh* pMesh = pScene->mMeshes[i];
		std::string meshName = std::string(pMesh->mName.C_Str());

		processMeshVertices(pMesh, vert, norm, text);
		processMeshFaces(pMesh, indices,vertexOffset);
		vertToBone.resize(vertToBone.size() + pMesh->mNumVertices);
		processMeshBones(pMesh, obj, vertToBone, obj->boneNameToID, vertexOffset);


		Mesh* mesh = new Mesh(meshName, loadMaterial(pScene->mMaterials[pMesh->mMaterialIndex]), indexOffset, pMesh->mNumFaces * 3);
		obj->meshList.push_back(mesh);

		indexOffset = indexOffset + pMesh->mNumFaces * 3;
		vertexOffset = vertexOffset + pMesh->mNumVertices;
	}

	obj->root = new Node(pScene->mRootNode);
	if(pScene->HasAnimations())
		processAnimations(pScene, obj, obj->boneNameToID);

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

	//if there are bones, pass the data to GPU
	if (obj->boneDataList.size() > 1) {
		GLuint vertToBoneBufferObject;
		glcheck(glGenBuffers(1, &vertToBoneBufferObject));
		glcheck(glBindBuffer(GL_ARRAY_BUFFER, vertToBoneBufferObject));
		glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(BonesForVertex) * vertToBone.size(), vertToBone.data(), GL_STATIC_DRAW));
		glcheck(glEnableVertexAttribArray(3)); //Bone ID 1
		glcheck(glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(BonesForVertex), 0));
		glcheck(glEnableVertexAttribArray(4)); //Bone ID 2
		glcheck(glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(BonesForVertex), (const GLvoid*)(4 * sizeof(uint32_t))));
		glcheck(glEnableVertexAttribArray(5)); //weights ID 1
		glcheck(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(BonesForVertex),
															(const GLvoid*)(MAX_NUM_BONE_PER_VERTEX * sizeof(uint32_t))));
		glcheck(glEnableVertexAttribArray(6)); //weights ID 2
		glcheck(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(BonesForVertex),
			                                                (const GLvoid*)(MAX_NUM_BONE_PER_VERTEX * sizeof(uint32_t) + 4 * sizeof(float))));
	}

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

void ResourceManager::processMeshVertices(aiMesh* pMesh, std::vector<glm::vec3>& vert, std::vector<glm::vec3>& norm, std::vector<glm::vec2>& text) {
	aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (unsigned int j = 0; j < pMesh->mNumVertices; j++) {
		vert.push_back(glm::vec3(pMesh->mVertices[j].x, pMesh->mVertices[j].y, pMesh->mVertices[j].z));
		//because "aiProcess_GenSmoothNormals" flag above we can always explect normals
		norm.push_back(glm::vec3(pMesh->mNormals[j].x, pMesh->mNormals[j].y, pMesh->mNormals[j].z));
		aiVector3D textCoord = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][j] : Zero3D;
		text.push_back(glm::vec2(textCoord.x, textCoord.y));
	}
}

void ResourceManager::processMeshFaces(aiMesh* pMesh, std::vector<glm::ivec3>& indices, int vertexOffset) {
	for (unsigned int j = 0; j < pMesh->mNumFaces; j++) {
		aiFace face = pMesh->mFaces[j];
		//need to offset the index by the number of vertices in the previous meshes because the indices for a mesh
		//are local to a mesh, so they all start from 0.
		indices.push_back(glm::ivec3(face.mIndices[0] + vertexOffset, face.mIndices[1] + vertexOffset, face.mIndices[2] + vertexOffset));
	}
}

void ResourceManager::processMeshBones(aiMesh* pMesh, Object* obj, std::vector<BonesForVertex>& vertToBone, 
										std::unordered_map<std::string, unsigned int>& boneNameToID, int vertexOffset) {
	for (unsigned int j = 0; j < pMesh->mNumBones; j++) {
		aiBone* aBone = pMesh->mBones[j];
		//if this bone has not been processed
		if (boneNameToID.find(aBone->mName.C_Str()) == boneNameToID.end()) {
			boneNameToID[aBone->mName.C_Str()] = (unsigned int)(obj->boneDataList.size()); //plus 1 to skip the 0th index because the default is 0
			Bone* boneData = new Bone(MathConverter::getMat4FromAiMatrix(pMesh->mBones[j]->mOffsetMatrix));
			obj->boneDataList.push_back(boneData);
		}
		int boneID = boneNameToID[aBone->mName.C_Str()];
		for (unsigned int k = 0; k < aBone->mNumWeights; k++) {
			vertToBone[aBone->mWeights[k].mVertexId + vertexOffset].addBoneWeight(boneID, aBone->mWeights[k].mWeight);
		}
	}
}

void ResourceManager::processAnimations(const aiScene* pScene, Object* obj, std::unordered_map<std::string, unsigned int>& boneNameToID) {
	for (unsigned i = 0; i < pScene->mNumAnimations; i++) {
		SkeletalAnimation* anim = new SkeletalAnimation(obj->root, pScene->mAnimations[i], boneNameToID);
		obj->animations.push_back(anim);
		obj->animationMap[pScene->mAnimations[i]->mName.C_Str()] = i;
	}
}

void ResourceManager::fixOrientation(const aiScene* pScene) {
	if (pScene->mMetaData)
	{
		int32_t UpAxis = 1, UpAxisSign = 1, FrontAxis = 2, FrontAxisSign = 1, CoordAxis = 0, CoordAxisSign = 1;
		double UnitScaleFactor = 1.0;
		for (unsigned MetadataIndex = 0; MetadataIndex < pScene->mMetaData->mNumProperties; ++MetadataIndex)
		{
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "UpAxis") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, UpAxis);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "UpAxisSign") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, UpAxisSign);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "FrontAxis") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, FrontAxis);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "FrontAxisSign") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, FrontAxisSign);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "CoordAxis") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, CoordAxis);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "CoordAxisSign") == 0)
			{
				pScene->mMetaData->Get<int32_t>(MetadataIndex, CoordAxisSign);
			}
			if (strcmp(pScene->mMetaData->mKeys[MetadataIndex].C_Str(), "UnitScaleFactor") == 0)
			{
				pScene->mMetaData->Get<double>(MetadataIndex, UnitScaleFactor);
			}
		}

		aiVector3D upVec, forwardVec, rightVec;

		upVec[UpAxis] = UpAxisSign * (float)UnitScaleFactor;
		forwardVec[FrontAxis] = FrontAxisSign * (float)UnitScaleFactor;
		rightVec[CoordAxis] = CoordAxisSign * (float)UnitScaleFactor;

		aiMatrix4x4 mat(rightVec.x, rightVec.y, rightVec.z, 0.0f,
			upVec.x, upVec.y, upVec.z, 0.0f,
			forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		pScene->mRootNode->mTransformation = mat;
	}
}

std::string ResourceManager::getFileNameFromPath(std::string& fPath) {
    return fPath.substr(fPath.rfind("/") + 1, std::string::npos);
}

std::string ResourceManager::getFolderPath(std::string& fPath) {
    return fPath.substr(0, fPath.rfind("/") + 1);
}
