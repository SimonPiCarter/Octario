#include "DrawableFactory.h"

using namespace glm;

DrawableModel DrawableFactory::createCubeDataModel(float size, float r, float g, float b) {
	return createCubeDataModel(size, vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b),vec3(r,g,b));
}


DrawableModel DrawableFactory::createCubeDataModel(float size, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, glm::vec3 color4,
	glm::vec3 color5, glm::vec3 color6) {
	DrawableModel model;
	model.textures = NULL;
	// Vertices
	model.vertices = new float[108]{-size, -size, -size,   size, -size, -size,   size, size, -size,     	   // Face 1
						   -size, -size, -size,   -size, size, -size,   size, size, -size,     // Face 1

						   size, -size, size,   size, -size, -size,   size, size, -size,       // Face 2
						   size, -size, size,   size, size, size,   size, size, -size,         // Face 2

						   -size, -size, size,   size, -size, size,   size, -size, -size,      // Face 3
						   -size, -size, size,   -size, -size, -size,   size, -size, -size,    // Face 3

						   -size, -size, size,   size, -size, size,   size, size, size,        // Face 4
						   -size, -size, size,   -size, size, size,   size, size, size,        // Face 4

						   -size, -size, -size,   -size, -size, size,   -size, size, size,     // Face 5
						   -size, -size, -size,   -size, size, -size,   -size, size, size,     // Face 5

						   -size, size, size,   size, size, size,   size, size, -size,         // Face 6
						   -size, size, size,   -size, size, -size,   size, size, -size};      // Face 6
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

DrawableModel DrawableFactory::createCubeSampleTextureDataModel(float size) {
	DrawableModel model = createCubeDataModel(size,0,0,0);
	model.textures = new float[72]{0, 0,   1, 0,   1, 1,     // Face 1
                               0, 0,   0, 1,   1, 1,     // Face 1

                               0, 0,   1, 0,   1, 1,     // Face 2
                               0, 0,   0, 1,   1, 1,     // Face 2

                               0, 0,   1, 0,   1, 1,     // Face 3
                               0, 0,   0, 1,   1, 1,     // Face 3

                               0, 0,   1, 0,   1, 1,     // Face 4
                               0, 0,   0, 1,   1, 1,     // Face 4

                               0, 0,   1, 0,   1, 1,     // Face 5
                               0, 0,   0, 1,   1, 1,     // Face 5

                               0, 0,   1, 0,   1, 1,     // Face 6
                               0, 0,   0, 1,   1, 1};    // Face 6
	model.sizeTexture = 72;
	model.texture = Texture("Textures/Caisse.jpg");
	model.texture.load();

	return model;
}
