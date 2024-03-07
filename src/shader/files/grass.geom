#version 330 core

#define AMOUNT 9 

layout (points) in;
layout (triangle_strip, max_vertices = AMOUNT) out;

uniform float size;
float scaledSize = 50 / size;

in mat4 matrices[1];

float allHeights[9];
out float height;
const float maxHeight = 0.2;

vec4 points[5] = vec4[](
    vec4(0.01 * scaledSize, 0.0, 0.0, 0.0), //Bottom left
    vec4(-0.01 * scaledSize, 0.0, 0.0, 0.0), //Bottom right
    vec4(0.005 * scaledSize, maxHeight / 2 * scaledSize, 0.0, 0.0), //Top left
    vec4(-0.005 * scaledSize, maxHeight / 2 * scaledSize, 0.0, 0.0), //Top right
    vec4(0.0, maxHeight * scaledSize, 0.0, 0.0) //Top
);

void spawn(vec4 origin) {    
    //Translate all points with the (view and projection) matrices and origin
    //Save all heights in an array for the next step
    for (int i = 0; i < 5; i++) {
        allHeights[i] = 1 - (points[i].y / maxHeight / scaledSize);
        points[i] = matrices[0] * (points[i] + origin);
    }
    
    //Emit grassblade vertices
    //Assign the variables for the fragment shader
    for (int i = 0; i < AMOUNT / 3; i++) {
        gl_Position = points[i];
        height = allHeights[i];
        EmitVertex();

        gl_Position = points[i + 1];
        height = allHeights[i + 1];
        EmitVertex();

        gl_Position = points[i + 2];
        height = allHeights[i + 2];
        EmitVertex();
        EndPrimitive();
    }
}

void main() {
    spawn(gl_in[0].gl_Position);
    //    spawn(gl_in[0].gl_Position + vec4(0.2, 0.0, 0.0, 0.0));
}