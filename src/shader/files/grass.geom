#version 330 core

#define MAX_LAYERS 11
#define PI 3.14159265359 

//In / Out
layout (points) in;
layout (triangle_strip, max_vertices = 63) out; //MAX_LAYERS * 6 - 3

in mat4 matrices[1];
in vec2 wind[1];
in int size[1];

out float height;
uniform float maxHeight;

uniform int layers;
uniform vec3[(MAX_LAYERS + 1) * 2 - 1] pointsRaw;
vec4[(MAX_LAYERS + 1) * 2 - 1] points;

uniform int useRandomOffset;

out vec3 fragNormal;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec4 origin = gl_in[0].gl_Position;
    vec2, raw2D = vec2(origin.x, origin.z);
    if (useRandomOffset == 1) origin += vec4((rand(raw2D) - 0.5) / size[0], 0, (rand(raw2D) - 0.5) / size[0], 0);

    //Precalculate all points
    for (int i = 0; i < (MAX_LAYERS + 1) * 2 - 1; i++) {
        vec3 raw = pointsRaw[i];
        float factor = pow(raw.y / maxHeight, 2.5) / (size[0] / 10.0);

        points[i] = vec4(raw.x + wind[0].x * factor, raw.y / (size[0] / 250.0), raw.z + wind[0].y * factor, 1);
        points[i] += origin;
    }

    //Loop through for emitting vertices and normals
    int i = 0;
    int counter = 0;
    while (i <= layers * 2) {
        //Point
        gl_Position = matrices[0] * points[i];

        //Normal
        vec3 prev = vec3(points[i]);
        vec3 next = vec3(points[i]);

        if (i > 2) prev = vec3(points[i - 2]);
        if (i <= layers * 2) next = vec3(points[i + 2]);

        fragNormal = normalize(abs(next - prev));

        //Height
        height = pointsRaw[i].y / maxHeight;
        
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