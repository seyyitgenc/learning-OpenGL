#version 460 core

in vec2 TexCoord;
// in vec3 ourColor;

out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float blend;

void main() { FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blend); /* vec4(ourColor, 1.0f);*/ }