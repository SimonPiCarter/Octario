// Version du GLSL

#version 330 core


// Entrées

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Color;


// Uniform

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;


// Sortie

out vec3 color;
out vec3 pos_world;
out vec3 normal;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = mvp * vec4(in_Vertex, 1.0);

    pos_world = (model * vec4(in_Vertex, 1.0)).xyz;
	vec3 vertexPosition_cameraspace = ( view * model * vec4(pos_world,1)).xyz;

	normal = (model * vec4(in_Normal, 0.0)).xyz;

    // Envoi de la couleur au Fragment Shader

    color = in_Color;
}
