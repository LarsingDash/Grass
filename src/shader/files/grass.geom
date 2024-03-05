#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

in mat4 grassProjMat[1];
in mat4 grassViewMat[1];

void main() {
    vec4 origin = grassProjMat[0] * grassViewMat[0] * gl_in[0].gl_Position;
    
    gl_Position = origin;
    EmitVertex();

    gl_Position = origin + vec4(0, 0.1, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}  