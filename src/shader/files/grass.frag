#version 330 core
out vec4 FragColor;

in float height;
uniform int grassColor;

void main()
{
    float value = 0.4 + (0.2 * height);
    FragColor = vec4(
        0.0,
        grassColor == 1 ? 0.0 : value,
        grassColor == 0 ? 0.0 : value,
        1.0f
    );
}
