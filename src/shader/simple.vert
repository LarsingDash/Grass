#version 330 core
layout (location = 0) in vec3 vecPos;

void main()
{
    gl_Position = vec4(vecPos.x, vecPos.y, vecPos.z, 1.0);
}