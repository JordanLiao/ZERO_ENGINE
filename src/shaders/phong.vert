#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTex;

uniform mat4 projView;
uniform mat4 model;

out vec2 texCoord;
out vec3 normalOutput;
out vec3 posOutput;

void main()
{
    gl_Position = projView * model * vec4(position, 1.0);
    normalOutput = normalize(mat3(transpose(inverse(model))) * normal); 
    posOutput = vec3(model * vec4(position, 1.0));
    texCoord = aTex;
}