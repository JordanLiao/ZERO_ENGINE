#include "Resources.h"

Resources::Material::Material()
{
	materialName = "default";
	ambient = glm::vec3(0.2f, 0.01f, 0.01f); //ka
	diffuse = glm::vec3(0.6f, 0.0f, 0.0f);  //kd
	specular = glm::vec3(0.2f, 0.0f, 0.0f);  //ks
	specularFocus = 5.f;  //ns
	opticalDensity = 1.f; //ni
	nonTransparency = 1.0f;  //d
	illum = 0;  //type of illumination
	diffuseTexture = 0; //map_kd
}
