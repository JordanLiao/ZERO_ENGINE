#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in ivec4 boneID1;
layout (location = 4) in ivec4 boneID2;
layout (location = 5) in vec4 boneWeights1;
layout (location = 6) in vec4 boneWeights2;

uniform mat4 projView;
uniform mat4 boneTransforms[100];

out vec2 texCoord;
out vec3 normalOutput;
out vec3 posOutput;

void main() {
  mat4 trans  = boneTransforms[boneID1[0]] * boneWeights1[0];
       trans += boneTransforms[boneID1[1]] * boneWeights1[1]; 
       trans += boneTransforms[boneID1[2]] * boneWeights1[2]; 
       trans += boneTransforms[boneID1[3]] * boneWeights1[3];
       trans += boneTransforms[boneID2[0]] * boneWeights2[0];
       trans += boneTransforms[boneID2[1]] * boneWeights2[1]; 
       trans += boneTransforms[boneID2[2]] * boneWeights2[2]; 
       trans += boneTransforms[boneID2[3]] * boneWeights2[3];
  vec4 pos = trans * vec4(position, 1.0);
  gl_Position = projView * pos;
  normalOutput = normalize(mat3(transpose(inverse(trans))) * normal);
  posOutput = vec3(pos);
  texCoord = aTex;
}