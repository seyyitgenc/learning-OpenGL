#version 460 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;



void main() {
    float ambientStrenght = 0.1f;
    vec3 ambient = ambientStrenght * lightColor;
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0f);
}