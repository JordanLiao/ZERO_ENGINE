#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_OUT {
    vec4 fragPos;
    vec3 normal;
    vec2 texCoord;
    vec4 fragPosLightSpace;
} vs_out;

uniform mat4 projView;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{    
    vs_out.fragPos = model * vec4(position, 1.0);
    vs_out.normal = transpose(inverse(mat3(model))) * normal;
    vs_out.texCoord = texCoord;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vs_out.fragPos;
    gl_Position = projView * vs_out.fragPos;
}