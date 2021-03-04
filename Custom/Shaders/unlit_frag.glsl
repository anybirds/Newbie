#version 330 core

in vec2 _FRAG_UV;
uniform sampler2D _MAIN_TEX;
out vec4 _FRAG_COLOR;

void main() {
    _FRAG_COLOR = texture(_MAIN_TEX, _FRAG_UV);
}