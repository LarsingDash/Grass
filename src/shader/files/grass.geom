#version 330 core

#define AMOUNT 9 

layout (points) in;
layout (triangle_strip, max_vertices = AMOUNT) out;

in mat4 matrices[1];
float heights[9];
out float height;
const float maxHeight = 0.3;

void spawn(vec4 origin) {
    vec4[5] points = vec4[](
        vec4(0.02, 0.0, 0.0, 0.0), //Bottom left
        vec4(-0.02, 0.0, 0.0, 0.0), //Bottom right
        vec4(0.01, maxHeight / 2, 0.0, 0.0), //Top left
        vec4(-0.01, maxHeight / 2, 0.0, 0.0), //Top right
        vec4(0.0, maxHeight, 0.0, 0.0) //Top
    );

    for (int i = 0; i < 5; i++) {
        heights[i] = 1 - (points[i].y / maxHeight);
        points[i] = matrices[0] * (points[i] + origin);
    }
    
    for (int i = 0; i < AMOUNT / 3; i++) {
        gl_Position = points[i];
        height = heights[i];
        EmitVertex();

        gl_Position = points[i + 1];
        height = heights[i + 1];
        EmitVertex();

        gl_Position = points[i + 2];
        height = heights[i + 2];
        EmitVertex();
        EndPrimitive();
    }
}

void main() {
    spawn(gl_in[0].gl_Position);
    //    spawn(gl_in[0].gl_Position + vec4(0.2, 0.0, 0.0, 0.0));
}