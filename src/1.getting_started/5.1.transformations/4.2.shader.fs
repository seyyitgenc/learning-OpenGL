#version 460 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float var;
void main() {
    // clang-format off
    FragColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),var);
    //clang-format on
}