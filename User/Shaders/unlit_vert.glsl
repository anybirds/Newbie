#version 330 core

layout (location = 0) in vec3 _VERT_POS;
layout (location = 2) in vec2 _VERT_UV;

uniform mat4 _MODEL;
uniform mat4 _CAM;
uniform mat4 _NORM;

out vec2 _FRAG_UV;

void main() {
    _FRAG_UV = vec2(_VERT_UV.x, 1 - _VERT_UV.y);
    gl_Position = _NORM * inverse(_CAM) * _MODEL * vec4(_VERT_POS, 1.0);
}