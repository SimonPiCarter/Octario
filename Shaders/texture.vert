// Version du GLSL

#version 150 core


// Entrées

in vec3 in_Vertex;
in vec2 in_TexCoord0;
in vec3 in_Normal;


// Uniform

uniform mat4 mvp;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 lightPos;


// Sortie

out vec2 coordTexture;
out vec3 pos_world;
out vec3 normal;

// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = mvp * vec4(in_Vertex, 1.0);

    pos_world = (model * vec4(in_Vertex, 1.0)).xyz;
	vec3 vertexPosition_cameraspace = ( view * model * vec4(pos_world,1)).xyz;

	normal = in_Normal;

    // Envoi des coordonnées de texture au Fragment Shader

    coordTexture = in_TexCoord0;
}
