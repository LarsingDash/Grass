#version 330 core
layout (location = 0) in vec3 vecPos;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;

out vec3 fragNormal;

void main()
{
    fragNormal = normal;
    gl_Position = projection * view * vec4(vecPos, 1.0f);
}   