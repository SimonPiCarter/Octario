// Version du GLSL

#version 330


// Entrée

in vec2 coordTexture;
in vec3 pos_world;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 cameraPos_tangent;

// Uniform

uniform sampler2D colorTexture;
uniform int enableNormalMapping;
uniform sampler2D normalTexture;
uniform int enableBumpMapping;
uniform sampler2D bumpTexture;
uniform samplerCube shadowMap;
uniform vec4 pointLightPos[16];
uniform vec4 pointLightProp[16];
uniform int pointLightCount;

// Sortie 

out vec4 out_Color;

#define EPSILON 0.001

float calcShadowFactor(vec3 lightToFrag)
{
    float sampledDistance = texture(shadowMap, lightToFrag).r;

    float distance = length(lightToFrag);

    if (distance <= sampledDistance + EPSILON)
        return 1.0;
    else
        return 0.0;
}

vec3 computeNormal() {
	if ( enableNormalMapping == 0 ) {
		return normal;
	} else {
		vec3 coord = texture( normalTexture, coordTexture ).rgb*2.0 - 1.0;
		// Normal of the computed fragment, in camera space
		return normalize(coord.r*tangent + coord.g*bitangent + coord.b*normal);
	}
}

vec3 computeTexColor() {
	if ( enableBumpMapping == 0 ) {
		return texture( colorTexture, coordTexture ).rgb;
	} else {
		return vec3(0,0,0);
	}
}

vec3 computeColor() {
	// compute normal vector
	vec3 n = computeNormal();

	// Ambient : simulates indirect lighting
	vec3 color = computeTexColor();
	//vec3 color = texture( shadowMap, pos_world - pointLightPos[0].xyz ).rgb;
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

		float shadowFactor = 1.0;
		if ( i == 0 ) {
			vec3 lightDir = pos_world - pointLightPos[i].xyz;
			shadowFactor = calcShadowFactor(lightDir);
		}

		finalColor += shadowFactor * color * pointLightProp[i].rgb * pointLightProp[i].w * cosTheta / (distance*distance);
	}

	return finalColor;
}

// Fonction main

void main()
{
	vec3 finalColor = computeColor();

    // final color
    out_Color = vec4(finalColor, 1.0);
}
