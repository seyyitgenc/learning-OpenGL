#version 460 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrenght = 0.1f;
    float specularStrenght = 0.5f;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrenght * spec * lightColor;
    vec3 ambient = ambientStrenght * lightColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}