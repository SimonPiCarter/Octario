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
	unsigned int* ibo;
	int sizeIbo;

	Texture* texture;

	DrawableModel() : vertices(NULL), colors(NULL), textures(NULL), texture(NULL), normals(NULL) {}
};
