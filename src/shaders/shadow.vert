#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projView;
uniform mat4 model;

void main() {
    gl_Position = projView * model * vec4(position, 1.0);
}