#version 330 core

layout (location = 2) in vec3 position;

uniform mat4 grassProj;
uniform mat4 grassView;

out mat4 grassProjMat;
out mat4 grassViewMat;

void main() {
    grassProjMat = grassProj;
    grassViewMat = grassView;
    
    gl_Position = vec4(position, 1.0);
}
