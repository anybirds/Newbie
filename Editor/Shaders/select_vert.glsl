#version 330 core

layout (location = 0) in vec3 _VERT_POS;
layout (location = 4) in mat4 _MODEL;

uniform mat4 _VIEW;
uniform mat4 _NORM;

uniform int base;
flat out int id;

void main() {
    id = base + gl_InstanceID;
    gl_Position = _NORM * _VIEW * _MODEL * vec4(_VERT_POS, 1.0);
}