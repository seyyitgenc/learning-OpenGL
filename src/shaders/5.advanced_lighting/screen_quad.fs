#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float gamma = 2.2;
void main()
{
    FragColor = texture(texture1, TexCoords);
    FragColor.rgb = pow(FragColor.rgb, vec3(1 / gamma));
}