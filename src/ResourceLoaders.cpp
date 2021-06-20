#include "ResourceLoaders.h"
#include "tools/stb_image.h"

GLuint ResourceLoaders::loadTexture(const char* fPath)
{
	int ftw, fth, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fPath, &ftw, &fth, &channels, STBI_rgb_alpha);
	if (data == NULL) {
		std::cout << "cannot load texture at " << fPath << std::endl;
		return false;
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

void ResourceLoaders::loadMaterial(std::unordered_map<std::string, Resources::Material*>&, const char* fName)
{
}
