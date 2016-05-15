#pragma once

#include "../Texture/Texture.h"

struct DrawableModel {
	float* vertices;
	int sizeVertices;
	float* colors;
	int sizeColors;
	float* textures;
	int sizeTexture;
	float* normals;
	int sizeNormals;
	float* tangents;
	int sizeTangents;
	float* bitangents;
	int sizeBitangents;
	unsigned int* ibo;
	int sizeIbo;

	Texture* texture;
	Texture* normalTexture;
	Texture* bumpTexture;

	DrawableModel() :
	    vertices(NULL),
	    sizeVertices(0),
	    colors(NULL),
	    sizeColors(0),
	    textures(NULL),
	    sizeTexture(0),
	    normals(NULL),
	    sizeNormals(0),
	    tangents(NULL),
	    sizeTangents(0),
	    bitangents(NULL),
	    sizeBitangents(0),
	    texture(NULL),
	    normalTexture(NULL),
	    bumpTexture(NULL) {}
};
