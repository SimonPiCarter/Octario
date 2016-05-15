// Version du GLSL

#version 330


// Input
in vec3 color;


// Output 
out vec4 out_Color;


// Fonction main

void main()
{
    // Couleur finale du pixel

    out_Color = vec4(color, 1.0);
}
