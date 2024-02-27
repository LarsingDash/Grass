#version 330 core
layout (location = 0) in vec3 vecPos;

out float test;

uniform mat4 projection;
uniform mat4 view;

void main()
{
//    mat4 compare = mat4(1.0f);
//    test = view == compare ? 0 : 1;
//    gl_Position = vec4(vecPos, 1.0f);
//    gl_Position = view * vec4(vecPos, 1.0f);
//    gl_Position = projection * vec4(vecPos, 1.0f);
    gl_Position = projection * view * vec4(vecPos, 1.0f);
}   