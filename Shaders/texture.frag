// Version du GLSL

#version 330 core


// Entr�e

in vec2 coordTexture;
in vec3 pos_world;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

// Uniform

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform mat4 model;
uniform mat4 view;
uniform vec4 pointLightPos[16];
uniform vec4 pointLightProp[16];
uniform int pointLightCount;

// Sortie 

out vec4 out_Color;


// Fonction main

void main()
{
	vec3 coord = texture( normalTexture, coordTexture ).rgb*2.0 - 1.0;
	// Normal of the computed fragment, in camera space
	vec3 n = normalize(coord.r*tangent + coord.g*bitangent + coord.b*normal);
	
	// Ambient : simulates indirect lighting
	vec3 color = texture(colorTexture, coordTexture).rgb;
	vec3 finalColor = 0.4*color;

	// Diffuse : "color" of the object
	for ( int i = 0 ; i < pointLightCount ; ++ i ) {
		// Distance to the light
		float distance = length( pointLightPos[i].xyz - pos_world );

		// Direction of the light (from the fragment to the light)
		vec3 l = normalize( pointLightPos[i].xyz - pos_world );
		// Cosine of the angle between the normal and the light direction, 
		// clamped above 0
		//  - light is at the vertical of the triangle -> 1
		//  - light is perpendicular to the triangle -> 0
		//  - light is behind the triangle -> 0
		float cosTheta = clamp( dot( n,l ), 0,1 );

		finalColor += color * pointLightProp[i].xyz * pointLightProp[i].w * cosTheta / (distance*distance);
	}

    // final color
    out_Color = vec4(finalColor, 1.0);
}
