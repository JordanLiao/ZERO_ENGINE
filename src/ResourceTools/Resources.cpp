#include "Resources.h"

Resources::Material::Material()
{
	materialName = "";
	ambient = glm::vec3(0.f); //ka
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);  //kd
	specular = glm::vec3(0.f);  //ks
	specularFocus = 0.f;  //ns
	opticalDensity = 1.f; //ni
	nonTransparency = 1.0f;  //d
	illum = 0;  //type of illumination
	diffuseTexture = 0; //map_kd
}
