#version 330 core

in int id;
out int _FRAG_INT;

void main() {
    _FRAG_INT = id;
}