#version 330 core

uniform sampler2D shadowMap;

in vec2 texCoord;
in vec3 normalOutput;
in vec3 posOutput;

out vec4 fragColor;

void main() {
    float depthValue = texture(shadowMap, texCoord).r;
    fragColor = vec4(vec3(depthValue), 1.0);
    //fragColor = vec4(vec3(texCoord.x),1.0);
}