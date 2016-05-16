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
uniform samplerCube shadowMap[16];
uniform int shadowMapCount;
uniform vec4 pointLightPos[16];
uniform vec4 pointLightProp[16];
uniform int pointLightCount;

// Sortie 

out vec4 out_Color;

#define EPSILON 0.001

float calcShadowFactor(vec3 lightToFrag, samplerCube shadowMap_p)
{
    float sampledDistance = texture(shadowMap_p, lightToFrag).r;

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
		float height = texture2D(bumpTexture, coordTexture).r;
		//Our heightmap only has one color channel.
		float v = height * 0.04 - 0.02;
		vec3 eye = normalize(cameraPos_tangent);
		vec2 newCoords = coordTexture + (eye.xy * v);
		return texture( colorTexture, newCoords ).rgb;
	}
}

float computeShadowFactor(int i) {
	float shadowFactor = 1.0;
	if ( i < shadowMapCount ) {
		vec3 lightDir = pos_world - pointLightPos[i].xyz;
		if ( i == 0 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[0]);
		} else if ( i == 1 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[1]);
		} else if ( i == 2 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[2]);
		} else if ( i == 3 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[3]);
		} else if ( i == 4 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[4]);
		} else if ( i == 5 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[5]);
		} else if ( i == 6 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[6]);
		} else if ( i == 7 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[7]);
		} else if ( i == 8 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[8]);
		} else if ( i == 9 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[9]);
		} else if ( i == 10 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[10]);
		} else if ( i == 11 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[11]);
		} else if ( i == 12 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[12]);
		} else if ( i == 13 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[13]);
		} else if ( i == 14 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[14]);
		} else if ( i == 15 ) {
			shadowFactor = calcShadowFactor(lightDir, shadowMap[15]);
		}
	}
	return shadowFactor;
}

vec3 computeColor() {
	// compute normal vector
	vec3 n = computeNormal();

	// Ambient : simulates indirect lighting
	vec3 color = computeTexColor();
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

		float shadowFactor = computeShadowFactor(i);

		finalColor += 0.4 * shadowFactor * color * pointLightProp[i].rgb * pointLightProp[i].w * cosTheta / (distance*distance);
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
