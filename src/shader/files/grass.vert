#version 330 core

layout (location = 2) in vec3 position;

uniform mat4 grassProj;
uniform mat4 grassView;

out mat4 matrices;

void main() {
    matrices = grassProj * grassView;
    
    gl_Position = vec4(position.x, position.y - 0.01, position.z, 1.0);
}
