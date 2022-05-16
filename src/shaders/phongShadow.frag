#version 330 core

uniform vec3 viewPos;        //just the eye pos
uniform vec3 diffuseFactor; //kd
uniform vec3 ambientColor; //ka
uniform vec3 specColor;  //ks
uniform float specHighlight; //ns, the larger this value is, the more apparent the light dot on the surface
uniform float dissolve; //d

//lights
uniform vec3 directionalLightDir;
uniform vec3 pointLightPos;
uniform vec3 lightColor;

uniform sampler2D shadowMap;
//uniform sampler2DShadow shadowMap;

in VS_OUT {
    vec4 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLightSpace;
} fs_in;

out vec4 fragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if(fragPosLightSpace.z > 1.0) {
		return 0.0;
	}
	vec2 shadowCoords;
	shadowCoords.x = projCoords.x * 0.5 + 0.5;
	shadowCoords.y = projCoords.y * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, shadowCoords).r;
    float currentDepth = projCoords.z * 0.5 + 0.5;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    
	float shadowValue = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadowValue;
}

void main()
{
	vec3 lightDir = normalize(-directionalLightDir);
	vec3 norm = normalize(fs_in.normal);
	
	//diffuse lighting
	float diffStrength = max(dot(norm, lightDir), 0.0); // this calculates diffuse intensity based on angle
	vec3 diffuse = lightColor * diffStrength * diffuseFactor * 1.3; 

	//specular
	vec3 viewDir = normalize(viewPos - fs_in.fragPos.xyz);	
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = 0.0;
	if(specHighlight > 0.0) { // if specHighlight is < 0, pow might produce undefined result if base is also 0
		spec = pow(max(dot(viewDir, reflectDir), 0.0), specHighlight);
	}
	vec3 specular = spec * specColor * lightColor * 0.5;  
	
 	float shadow = ShadowCalculation(fs_in.fragPosLightSpace, lightDir, norm);       
    vec3 result = ambientColor * 0.1 * lightColor + (diffuse + specular)*(1-shadow);
	fragColor = vec4(result, 1);
}