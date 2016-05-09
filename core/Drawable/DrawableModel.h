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

	DrawableModel() : vertices(NULL), colors(NULL), textures(NULL), normals(NULL), tangents(NULL), bitangents(NULL), texture(NULL), normalTexture(NULL) {}
};
