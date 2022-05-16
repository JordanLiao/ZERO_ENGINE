#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <string>

#include "../Core.h"

enum class shaderType { 
	vertex, 
	fragment 
};

enum shaderRole {
	simpleModelShader,
	phongShader,
	phongShadowShader,
	colorPickingShader,
	shadowMapShader,
	shadowInspectionShader,
	shaderSize
};

class Shader {
private: 
	static Shader* currBoundShader;
	GLuint programID;
	std::unordered_map<std::string, GLint> uniformMap;

	//methods
	GLuint LoadSingleShader(const char* shaderFilePath, shaderType type);
	GLint getUniformLocation(const std::string&);

public :
	Shader(const char* vertex_file_path, const char* fragment_file_path);
	~Shader();
	GLuint getId() const;
	bool setUniformMat4(const std::string&, glm::mat4);
	bool setUniformVec3(const std::string&, glm::vec3);
	bool setUniform1I(const std::string&, GLint);
	bool setUniorm1F(const std::string&, GLfloat);
	bool setUniorm4F(const std::string&, GLfloat, GLfloat, GLfloat, GLfloat);
	void bind();
	void unbind();
	bool isBound();
};


#endif
