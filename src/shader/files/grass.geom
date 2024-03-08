#version 330 core

#define MAX_LAYERS 7

//In / Out
layout (points) in;
layout (triangle_strip, max_vertices = 39) out; //MAX_LAYERS * 6 - 3

in mat4 matrices[1];
out float height;
uniform int layers;
uniform vec3[(MAX_LAYERS + 1) * 2 - 1] pointsRaw;
vec4[(MAX_LAYERS + 1) * 2 - 1] points;

out vec3 fragNormal;

void main() {
    //Precalculate all points
    for (int i = 0; i < (MAX_LAYERS + 1) * 2 - 1; i++) {
        points[i] = matrices[0] * (vec4(pointsRaw[i], 1) + gl_in[0].gl_Position);
    }

    //Loop through for emitting vertices
    int i = 0;
    int counter = 0;
    while (i <= layers * 2) {
        //Point
        gl_Position = points[i];
        
        //Normal
        vec3 prev = pointsRaw[i];
        vec3 next = pointsRaw[i];
        
        if (i > 2) prev = pointsRaw[i - 2];
        if (i <= layers * 2) next = pointsRaw[i + 2];
        
        fragNormal = normalize(abs(next - prev));

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