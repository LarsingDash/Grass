#version 330 core

#define MAX_LAYERS 7

//In / Out
layout (points) in;
layout (triangle_strip, max_vertices = 39) out; //MAX_LAYERS * 6 - 3

in mat4 matrices[1];
out float height;
uniform int layers;
uniform vec3[(MAX_LAYERS + 1) * 2 - 1] points;

void main() {
    int i = 0;
    int counter = 0;
    while (i <= layers * 2) {
        gl_Position = matrices[0] * (vec4(points[i], 1) + gl_in[0].gl_Position);
        EmitVertex();

        i++;
        counter++;
        if (counter == 3) {
            counter = 0;
            EndPrimitive();
            i -= 2;
        }
    }
}