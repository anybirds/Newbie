#version 330 core

uniform vec4 _ID
out vec4 _FRAG_COLOR;

void main() {
    _FRAG_COLOR = _ID;
}