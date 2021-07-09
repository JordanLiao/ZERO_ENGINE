#version 330 core

uniform vec3 color;

in vec3 normalOutput;
in vec3 posOutput;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
	vec3 color = vec3(0.0, 1.0, 0.0);
    	fragColor = vec4(color, 1.0);
}