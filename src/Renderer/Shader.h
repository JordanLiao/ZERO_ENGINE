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

enum ShaderType { vertex, fragment };

class Shader {
	enum shader {
		simpleModelShader,
		phongShader
	};

private: 
	GLuint programID;
	std::unordered_map<std::string, GLint> uniformMap;
	glm::mat4 *proj;
	glm::mat4  *view;

	//methods
	GLuint LoadSingleShader(const char* shaderFilePath, ShaderType type);
	GLint getUniformLocation(const std::string&);

public :
	Shader(const char* vertex_file_path, const char* fragment_file_path, glm::mat4* projMat,
		glm::mat4* viewMat);
	~Shader();
	GLuint getId() const;
	bool setUniformMat4(const std::string&, glm::mat4);
	bool setUniformVec3(const std::string&, glm::vec3);
	bool setUniform1I(const std::string&, GLint);
	bool setUniorm1F(const std::string&, GLfloat);
	void bind();
	void unbind() const;
};


#endif
