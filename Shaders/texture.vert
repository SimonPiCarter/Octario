// Version du GLSL

#version 330


// Entrées
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Tangent;
layout(location = 3) in vec3 in_Bitangent;
layout(location = 4) in vec2 in_TexCoord0;


// Uniform

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform vec3 camPos_world;

// Sortie

out vec2 coordTexture;
out vec3 pos_world;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec3 cameraPos_tangent;

// Fonction main

void main()
{
    // Position finale du vertex en 3D

    gl_Position = mvp * vec4(in_Vertex, 1.0);

    pos_world = (model * vec4(in_Vertex, 1.0)).xyz;

	normal = (model * vec4(in_Normal, 0.0)).xyz;
	tangent = (model * vec4(in_Tangent, 0.0)).xyz;
	bitangent = (model * vec4(in_Bitangent, 0.0)).xyz;

	// 
	mat3 TBNMatrix = mat3(tangent, bitangent, normal);
	cameraPos_tangent = camPos_world - gl_Position.xyz;
	cameraPos_tangent *= TBNMatrix; 

    // Sending coord pos
    coordTexture = in_TexCoord0;
}
