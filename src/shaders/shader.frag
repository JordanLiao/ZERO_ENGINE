#version 330 core

uniform vec4 color;

in vec3 normalOutput;
in vec3 posOutput;

out vec4 fragColor;

void main()
{
    //fragColor = color;
    fragColor = vec4(0.5, 0.0, 0.0, 1.0);
}