#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include "Core.h"

namespace Resources {
	typedef struct {
		glm::vec3 ambient; //ka
		glm::vec3 diffuse;  //kd
		glm::vec3 specular;  //ks
		GLfloat specularFocus;  //ns
		GLfloat nonTransparency;  //d
		GLuint illum;  //type of illumination
		std::string diffuseTexuture; //map_kd
	} Material;
}

#endif
