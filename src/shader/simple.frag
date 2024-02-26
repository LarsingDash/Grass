#version 330 core
out vec4 FragColor;
in float test;

void main()
{
    FragColor = vec4(test, 0.5f, 0.2f, 1.0f);
}