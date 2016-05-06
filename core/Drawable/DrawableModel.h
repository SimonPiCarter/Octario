#pragma once

struct DrawableModel {
	float* vertices;
	int sizeVertices;
	float* colors;
	int sizeColors;
	unsigned int* ibo;
	int sizeIbo;
};
