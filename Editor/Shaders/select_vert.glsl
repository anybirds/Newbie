#version 330 core

layout (location = 0) in vec3 _VERT_POS;

uniform mat4 _MODEL;
uniform mat4 _VIEW;
uniform mat4 _NORM;

void main() {
    gl_Position = _NORM * inverse(_VIEW) * _MODEL * vec4(_VERT_POS, 1.0);
}