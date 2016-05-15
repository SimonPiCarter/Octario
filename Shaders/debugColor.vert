// Version du GLSL

#version 330


// Input
in vec3 in_Vertex;
in vec3 in_Color;

// Uniform

uniform mat4 mvp;

// Output
out vec3 color;


// Fonction main

void main()
{
    gl_Position = mvp * vec4(in_Vertex, 1.0);

    color = in_Color;
}