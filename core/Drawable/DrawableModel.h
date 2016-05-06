#pragma once

#include "../Texture/Texture.h"

struct DrawableModel {
	float* vertices;
	int sizeVertices;
	float* colors;
	int sizeColors;
	float* textures;
	int sizeTexture;
	unsigned int* ibo;
	int sizeIbo;

	Texture* texture;
};
