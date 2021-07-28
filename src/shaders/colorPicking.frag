#version 330 core

uniform vec4 colorCode;

out vec4 fragColor;

void main()
{
    fragColor = colorCode;
}