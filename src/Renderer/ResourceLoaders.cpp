#include "ResourceLoaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"

GLuint ResourceLoaders::loadTexture(const char* fPath)
{
	int ftw, fth, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fPath, &ftw, &fth, &channels, STBI_rgb_alpha);
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
	
	return textId;
}

std::unordered_map<std::string, Resources::Material*>* ResourceLoaders::loadMaterialMap(const char* fName)
{
	std::unordered_map<std::string, Resources::Material*>* matMap = 
		new std::unordered_map<std::string, Resources::Material*>();
	
	std::ifstream objFile(fName);
	if (!objFile.is_open())
		std::cerr << "Can't open the file " << fName << std::endl;
	
	Resources::Material* curr = NULL;
	std::string line;
	while (std::getline(objFile, line))
	{
		std::stringstream ss;
		ss << line;
		std::string label;
		ss >> label;
		if (label == "#")
			continue;

		if (label == "newmtl")
		{
			if (curr != NULL) //insert the previous material if it is not NULL
			{
				//std::cout << "putting " << curr->materialName << std::endl;
				(*matMap)[curr->materialName] = curr;
			}
			curr = new Resources::Material;
			ss >> curr->materialName;
			//std::cout << "loading " << curr->materialName << std::endl;
		}

		if (curr == NULL)
			continue;

		if (label == "Ka")
		{
			ss >> curr->ambient.x >> curr->ambient.y >> curr->ambient.z;
		}

		if (label == "Kd")
		{
			ss >> curr->diffuse.x >> curr->diffuse.y >> curr->diffuse.z;
		}

		if (label == "Ks")
		{
			ss >> curr->specular.x >> curr->specular.y >> curr->specular.z;
		}

		if (label == "Ns")
		{
			ss >> curr->specularFocus;
		}

		if (label == "Ni")
		{
			ss >> curr->opticalDensity;
		}

		if (label == "d")
		{
			ss >> curr->nonTransparency;
		}

		if (label == "illum")
		{
			ss >> curr->illum;
		}
	}
	if (curr != NULL) //insert the previous material if it is not NULL
	{
		//std::cout << "putting " << curr->materialName << std::endl;
		(*matMap)[curr->materialName] = curr;
	}

	objFile.close();
	return matMap;
}


