#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

uniform sampler2D tex;
uniform vec3 viewPos;        //just the eye pose
uniform vec3 diffuseFactor; //kd
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
	vec3 lightDir = normalize(-directionalLightDir);
	vec3 norm = normalize(normalOutput);
	
	//diffuse lighting
	float diffStrength = max(dot(norm, lightDir), 0.0); // this calculates diffuse intensity based on angle
	vec3 diffuse = lightColor * diffStrength * diffuseFactor; 

	//specular
	vec3 viewDir = normalize(viewPos - posOutput);	
	vec3 reflectDir = reflect(-lightDir, normalOutput);  
	float spec = 0.0;
	if(specHighlight > 0.0) { // if specHighlight is < 0, pow might produce undefined result if base is also 0
		spec = pow(max(dot(viewDir, reflectDir), 0.0), specHighlight);
	}
	vec3 specular = spec * specColor * lightColor;  
	
	vec3 result = ambientColor * 0.05 * lightColor + diffuse * 1.3 + specular;
	//vec3 result = ambientColor + diffuse + specular;
	fragColor = vec4(result, 1);
	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
