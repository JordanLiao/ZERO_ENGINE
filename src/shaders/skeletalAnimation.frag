#version 330 core

in vec3 normalOutput;
in vec3 posOutput;
in vec2 texCoord;
out vec4 fragColor;

void main()
{
    //fragColor = color;
    fragColor = vec4(0.5, 0.0, 0.0, 1.0);
}