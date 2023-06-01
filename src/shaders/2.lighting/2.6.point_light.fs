#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main() {
    // ambient

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
   
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 result = attenuation * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0f);
}