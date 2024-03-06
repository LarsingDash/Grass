#version 330 core
out vec4 FragColor;

in float height;

void main()
{
    FragColor = vec4(0, 0.4 + (0.2 * height), 0, 1.0f);
}
