#version 330 core

in vec2 _FRAG_UV;
out vec4 _FRAG_COLOR;
uniform sampler2D sceneTexture;
uniform sampler2D whiteTexture;

void main() {
    bool outline = false;
    for (float i = -2.0; i <= 2.0; i += 1.0) {
        for (float j = -2.0; j <= 2.0; j += 1.0) {
            outline = outline || bool(texture(whiteTexture, _FRAG_UV + vec2(i, j) / vec2(textureSize(whiteTexture, 0))).r);
        }
    }
    outline = outline && !bool(texture(whiteTexture, _FRAG_UV).r);
    vec4 outlineColor = vec4(30.0 / 255.0, 144.0 / 255.0, 1.0, 1.0);
    _FRAG_COLOR = mix(texture(sceneTexture, _FRAG_UV), outlineColor, outline);
}