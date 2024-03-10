#version 330 core

layout (location = 2) in vec3 position;
layout (location = 3) in vec2 windData;

uniform mat4 grassProj;
uniform mat4 grassView;
uniform int gridSize;

out mat4 matrices;
out vec2 wind;
out int size;

void main() {
    matrices = grassProj * grassView;
    wind = windData;
    size = gridSize;
    
    gl_Position = vec4(position.x, position.y - 0.005 * (50.0 / gridSize), position.z, 0.0);
}
