#version 330 core
layout (location = 0) in vec3 vecPos;
out float test;

void main()
{
    test = 0;
    gl_Position = vec4(vecPos.x, vecPos.y, vecPos.z, 1.0);
}