#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 9) out;

in mat4 matrices[1];
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

    for (int i = 0; i < 5; i++)
    points[i] = matrices[0] * (points[i] + origin);

    for (int i = 0; i < 3; i++) {
        gl_Position = points[i];
        height = 1 - (points[i].y / maxHeight);
        EmitVertex();
        gl_Position = points[i + 1];
        height = 1 - (points[i].y / maxHeight);
        EmitVertex();
        gl_Position = points[i + 2];
        height = 1 - (points[i].y / maxHeight);
        EmitVertex();
        EndPrimitive();
    }
}

void main() {
    spawn(gl_in[0].gl_Position);
    //    spawn(gl_in[0].gl_Position + vec4(0.2, 0.0, 0.0, 0.0));
}