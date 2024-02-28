#version 330 core
layout (location = 0) in vec3 vecPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
//    gl_Position = vec4(vecPos, 1.0f);
    gl_Position = projection * view * vec4(vecPos, 1.0f);
}   