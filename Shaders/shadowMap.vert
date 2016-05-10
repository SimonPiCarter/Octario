// Version du GLSL

#version 330 core

layout(location = 0) in vec3 in_Vertex;

// Uniform

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;

out vec3 pos_world;
                  
void main()
{
    vec4 Pos4 = vec4(in_Vertex, 1.0);
    gl_Position = mvp * Pos4;
    pos_world = (view *  model * Pos4).xyz;
}
