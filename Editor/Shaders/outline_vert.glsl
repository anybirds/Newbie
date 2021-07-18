#version 330 core

layout (location = 0) in vec3 _VERT_POS;
layout (location = 2) in vec2 _VERT_UV;

uniform mat4 _MODEL;
uniform mat4 _VIEW;
uniform mat4 _NORM;

uniform vec4 uvScale;

out vec2 _FRAG_UV;

void main() {
    _FRAG_UV = _VERT_UV * uvScale.xy;
    gl_Position = _NORM * _VIEW * _MODEL * vec4(_VERT_POS, 1.0);
}