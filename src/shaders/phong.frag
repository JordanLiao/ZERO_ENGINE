#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

uniform sampler2D tex;
uniform vec3 viewPos;        //just the eye pose
uniform vec3 diffuseColor; //kd
uniform vec3 ambientColor; //ka
uniform vec3 specColor;  //ks
uniform float specHighlight; //ns, the larger this value is, the more apparent the light dot on the surface
uniform float dissolve; //d

//lights
uniform vec3 directionalLightDir;
uniform vec3 pointLightPos;
uniform vec3 lightColor;

// You can output many things. The first vec4 type output determines the color of the fragment
in vec3 normalOutput;
in vec3 posOutput;
in vec2 texCoord;
out vec4 fragColor;

void main()
{
	vec3 lightDir = -directionalLightDir;
	vec3 norm = normalize(normalOutput);
	
	//diffuse lighting
	float diffStrength = max(dot(norm, lightDir), 0.0); // this calculates diffuse intensity based on angle
	vec3 diffuse = diffStrength * diffuseColor; 

	//specular
	vec3 viewDir = normalize(viewPos - posOutput);
	vec3 halfVec = normalize(viewDir + lightDir);	
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = 0.0;
	if(specHighlight > 0.0) {
	  spec = pow(max(dot(halfVec, norm), 0.0), specHighlight);
	}
	vec3 specular = spec * specColor * (specHighlight + 2) / 6.283;  
	
	vec3 result = ambientColor * 0.1  + diffuse + specular;
	fragColor = vec4(result, 1);
}
