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

// You can output many things. The first vec4 type output determines the color of the fragment
in vec3 normalOutput;
in vec3 posOutput;
in vec2 texCoord;
out vec4 fragColor;

void main()
{
	//vec3 lightPos = vec3(10.0, 5.0, 0.0);	
	//vec3 lightDir = normalize(lightPos - posOutput);
	vec3 lightDir = normalize(directionalLightDir);
	vec3 norm = normalize(normalOutput);
	
	//diffuse lighting
	float diffStrenth = max(dot(norm, lightDir), 0.0); // this calculates diffuse intensity based on angle
	vec3 diffuse = diffStrenth * diffuseFactor * 2;   //color based on texture color value and input diffuse factor kd

	vec3 viewDir = normalize(viewPos - posOutput);	
	vec3 reflectDir = reflect(-lightDir, normalOutput);  
	float spec = 0.0;
	if(specHighlight > 0.0) { // if specHighlight is < 0, pow might produce undefined result if base is also 0
		spec = pow(max(dot(viewDir, reflectDir), 0.0), specHighlight);
	}
	vec3 specular = spec * specColor;  
	
	vec3 result = ambientColor * 0.1 + diffuse * 0.7 + specular * 0.5;
	//vec3 result = ambientColor * 0.2 + diffuse + specular;
	fragColor = vec4(result, 1);
}
