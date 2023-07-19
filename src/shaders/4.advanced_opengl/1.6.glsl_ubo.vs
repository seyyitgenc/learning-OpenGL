#version 460 core
layout (location = 0) in vec3 aPos;

// binding can be use aswell
// layout (std140 ,binding = 0) uniform Matrices
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
