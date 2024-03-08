#version 330 core

#define AMOUNT 9 

//In / Out
layout (points) in;
layout (triangle_strip, max_vertices = AMOUNT) out;

//Size
uniform float size;
float scaledSize = 50 / size;

//Transformation
in mat4 matrices[1];

//Height
float allHeights[5];
const float maxHeight = 0.2;

//Fragment Shader
out float height;

//GrassBlade points
vec4 points[5] = vec4[](
    vec4(0.01 * scaledSize, 0, 0, 0), //Bottom left
    vec4(-0.01 * scaledSize, 0, 0, 0), //Bottom right
    vec4(0.005 * scaledSize, maxHeight / 2 * scaledSize, 0, 0), //Top left
    vec4(-0.005 * scaledSize, maxHeight / 2 * scaledSize, 0, 0), //Top right
    vec4(0, maxHeight * scaledSize, 0, 0) //Top
);
vec3 normals[5];

void spawn(vec4 origin) {
    //Translate all points with the (view and projection) matrices and origin
    //Save all heights in an array for the next step
    for (int i = 0; i < 5; i++) {
        allHeights[i] = 1 - (points[i].y / maxHeight / scaledSize);
        points[i] = matrices[0] * (points[i] + origin);
    }

    //Calculate the normals of the 3 heights (bottom is always 0)


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