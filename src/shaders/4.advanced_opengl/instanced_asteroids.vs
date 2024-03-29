#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in mat4 instanceMatrix;

out VS_OUT
{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
}
vs_out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;

    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}
