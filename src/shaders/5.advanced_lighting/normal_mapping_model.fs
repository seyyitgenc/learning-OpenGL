#version 460 core

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 LightPos;
    vec3 ViewPos;
}
fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
out vec4 FragColor;

uniform bool isTBN;

void main()
{
    if (isTBN)
    {
        vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
        vec3 ambient = 0.2 * color;

        vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = vec3(0.2) * spec;
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
    else
    {
        vec3 ambient = 0.05 * vec3(texture(texture_diffuse1, fs_in.TexCoords));

        // diffuse
        vec3 norm = normalize(fs_in.Normal);
        vec3 lightDir = normalize(fs_in.LightPos - fs_in.FragPos);
        float diff = max(dot(norm, lightDir), 0.0f);
        vec3 diffuse = diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));

        // specular
        vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0);
        vec3 specular = vec3(0.3) * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result, 1.0f);
    }
}