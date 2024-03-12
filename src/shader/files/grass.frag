#version 330 core
out vec4 FragColor;

in float height;
in vec3 fragNormal;

uniform int grassColor;

void main()
{
    vec3 lightDirection = vec3(1, 1, 1);
    float lighting = max(0.0, dot(normalize(fragNormal), normalize(lightDirection)));

    float value = 0.2 + (0.3 * height) + (0.3 * lighting);

    FragColor = vec4(
        0.0,
        grassColor == 1 ? 0.0 : value,
        grassColor == 0 ? 0.0 : value,
        1.0f
    );
}
