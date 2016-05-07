#include "DrawableFactory.h"

using namespace glm;

DrawableModel DrawableFactory::createCubeModel(float size, float r, float g, float b) {
	return createCubeModel(size, vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b));
}


DrawableModel DrawableFactory::createCubeModel(float size, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, glm::vec3 color4,
	glm::vec3 color5, glm::vec3 color6) {
	size /= 2.f;
	DrawableModel model;
	// Vertices
	model.vertices = new float[108]{-size, -size, -size,   size, -size, -size,   size, size, -size,     	   // Face 1
						   -size, -size, -size,   size, size, -size,   -size, size, -size,     	// Face 1

						   size, -size, size,   size, size, -size,   size, -size, -size,       // Face 2
						   size, -size, size,   size, size, size,   size, size, -size,         // Face 2

						   -size, -size, size,   size, -size, size,   size, -size, -size,      // Face 3
						   -size, -size, size,   size, -size, -size,   -size, -size, -size,    // Face 3

						   -size, -size, size,   size, size, size,   size, -size, size,        // Face 4
						   -size, -size, size,   -size, size, size,   size, size, size,        // Face 4

						   -size, -size, -size,   -size, size, size,   -size, -size, size,     // Face 5
						   -size, -size, -size,   -size, size, -size,   -size, size, size,     // Face 5

						   -size, size, size,   size, size, size,   size, size, -size,         // Face 6
						   -size, size, size,   size, size, -size,   -size, size, -size};      // Face 6
	model.sizeVertices = 108;
	// Colors
	model.colors = new float[108]{
				color1.r,color1.g,color1.b, color1.r,color1.g,color1.b, color1.r,color1.g,color1.b,      		// Face 1
				color1.r,color1.g,color1.b, color1.r,color1.g,color1.b, color1.r,color1.g,color1.b,            // Face 1

				color2.r,color2.g,color2.b, color2.r,color2.g,color2.b, color2.r,color2.g,color2.b,            // Face 2
				color2.r,color2.g,color2.b, color2.r,color2.g,color2.b, color2.r,color2.g,color2.b,            // Face 2

				color3.r,color3.g,color3.b, color3.r,color3.g,color3.b, color3.r,color3.g,color3.b,            // Face 3
				color3.r,color3.g,color3.b, color3.r,color3.g,color3.b, color3.r,color3.g,color3.b,            // Face 3

				color4.r,color4.g,color4.b, color4.r,color4.g,color4.b, color4.r,color4.g,color4.b,            // Face 4
				color4.r,color4.g,color4.b, color4.r,color4.g,color4.b, color4.r,color4.g,color4.b,            // Face 4

				color5.r,color5.g,color5.b, color5.r,color5.g,color5.b, color5.r,color5.g,color5.b,            // Face 5
				color5.r,color5.g,color5.b, color5.r,color5.g,color5.b, color5.r,color5.g,color5.b,            // Face 5

				color6.r,color6.g,color6.b, color6.r,color6.g,color6.b, color6.r,color6.g,color6.b,            // Face 6
				color6.r,color6.g,color6.b, color6.r,color6.g,color6.b, color6.r,color6.g,color6.b};           // Face 6
	model.sizeColors = 108;

	model.ibo = new unsigned int[36]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
	model.sizeIbo = 36;

	return model;
}

DrawableModel DrawableFactory::createCubeSampleTextureModel(float size) {
	size /= 2.f;
	DrawableModel model;
	// Vertices
	model.vertices = new float[24]{-size, -size, -size,		// 0
									size, -size, -size,		// 1
									size, size, -size,		// 2
									-size, size, -size,		// 3
									-size, -size, size,		// 4
									size, -size, size,		// 5
									size, size, size,		// 6
									-size, size, size};		// 7
	model.sizeVertices = 24;

	model.textures = new float[16]{0,0,
								   1,0,
								   1,1,
								   0,1,
								   0,0,
								   1,0,
								   1,1,
								   0,1};
	model.sizeTexture = 16;
	model.texture = new Texture("Textures/Caisse.jpg");
	model.texture->load();

	model.ibo = new unsigned int[36]{0, 1, 2,    	// Face 1
									0, 2, 3,     	// Face 1
									5, 2, 1,		// Face 2
									5, 6, 2,		// Face 2
									4, 5, 1,		// Face 3
									4, 1, 0,		// Face 3
									4, 6, 5,		// Face 4
									4, 7, 6,		// Face 4
									0, 7, 4,		// Face 5
									0, 3, 7,		// Face 5
									7, 6, 2,		// Face 6
									7, 2, 3};		// Face 6
	model.sizeIbo = 36;

	return model;
}

DrawableModel DrawableFactory::createPlaneModel(float width, float height, float thickness, float r, float g, float b) {
	width /= 2;	height /= 2;	thickness /= 2;
	DrawableModel model;
	// Vertices
	model.vertices = new float[24]{-width, -height, -thickness,		// 0
									width, -height, -thickness,		// 1
									width, height, -thickness,		// 2
									-width, height, -thickness,		// 3
									-width, -height, thickness,		// 4
									width, -height, thickness,		// 5
									width, height, thickness,		// 6
									-width, height, thickness};		// 7
	model.sizeVertices = 24;

	model.colors = new float[24]{r,g,b,
								r,g,b,
								r,g,b,
								r,g,b,
								r,g,b,
								r,g,b,
								r,g,b,
								r,g,b};
	model.sizeColors = 24;

	model.normals = new float[24]{-1,-1,-1,
								1,-1,-1,
								1,1,-1,
								-1,1,-1,
								-1,-1,1,
								1,-1,1,
								1,1,1,
								-1,1,1};
	model.sizeNormals = 24;

	model.ibo = new unsigned int[36]{0, 1, 2,    	// Face 1
									0, 2, 3,     	// Face 1
									5, 2, 1,		// Face 2
									5, 6, 2,		// Face 2
									4, 5, 1,		// Face 3
									4, 1, 0,		// Face 3
									4, 6, 5,		// Face 4
									4, 7, 6,		// Face 4
									0, 7, 4,		// Face 5
									0, 3, 7,		// Face 5
									7, 6, 2,		// Face 6
									7, 2, 3};		// Face 6
	model.sizeIbo = 36;
	return model;
}
