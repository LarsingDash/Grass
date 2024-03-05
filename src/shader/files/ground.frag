#version 330 core
out vec4 FragColor;

in vec3 fragNormal;

void main()
{
    vec3 lightDirection = vec3(1, 1, 1);
    float lighting = max(0.0, dot(normalize(fragNormal), normalize(lightDirection)));

    FragColor = vec4(0, lighting, 0, 1.0f);
    //        FragColor = vec4(fragNormal.x, fragNormal.y / 6, fragNormal.z, 1.0f);
    //        FragColor = vec4(fragNormal.z >= 0.0 ? 1 : 0, 0, 0, 1.0f);
}
