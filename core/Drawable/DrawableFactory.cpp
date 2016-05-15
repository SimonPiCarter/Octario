#include "DrawableFactory.h"

#include <vector>

using namespace glm;

DrawableModel DrawableFactory::createCubeSampleTextureModel(float size) {
	return createCubeSampleTextureModel(size,"Textures/FullCaisse.jpg","Textures/FullCaisseNormal.jpg","");
}

DrawableModel DrawableFactory::createCubeSampleTextureModel(float size, std::string texture_p, std::string normalTexture_p, std::string bumpTexture_p) {
    size /= 2.f;
	DrawableModel model;
	// Vertices
	float* vertices_l = new float[144]{
        -size, -size, -size,  size, size, -size,  size, -size, -size,       // Face 1
        -size, -size, -size,   -size, size, -size,   size, size, -size,     // Face 1
        size, -size, size,   size, -size, -size,   size, size, -size,       // Face 2
        size, -size, size,   size, size, -size,  size, size, size,          // Face 2
        -size, -size, size,  size, -size, -size,   size, -size, size,       // Face 3
        -size, -size, size,  -size, -size, -size,  size, -size, -size,      // Face 3
        -size, -size, size,  size, -size, size,  size, size, size,          // Face 4
        -size, -size, size,  size, size, size,   -size, size, size,         // Face 4
        -size, -size, -size,   -size, -size, size,   -size, size, size,     // Face 5
        -size, -size, -size,   -size, size, size,  -size, size, -size,      // Face 5
        -size, size, size,   size, size, size,   size, size, -size,         // Face 6
        -size, size, size,   size, size, -size,  -size, size, -size         // Face 6
    };

	buildCubeDataInModel(vertices_l, model);

	loadTextures(texture_p,normalTexture_p,bumpTexture_p,model);

	computeTangents(model);

	return model;
}

DrawableModel DrawableFactory::createPlaneModel(float width, float height, float thickness, float r, float g, float b) {
    return createPlaneModel(width,height,thickness,r,g,b,"Textures/gray.png","Textures/grayNormal.png","");
}

DrawableModel DrawableFactory::createPlaneModel(float width, float height, float thickness, float r, float g, float b, std::string texture_p, std::string normalTexture_p, std::string bumpTexture_p) {

	width /= 2;	height /= 2;	thickness /= 2;
	DrawableModel model;

	// Vertices
	float* vertices_l = new float[144]{
        -width, -height, -thickness,  width, height, -thickness,  width, -height, -thickness,       // Face 1
        -width, -height, -thickness,   -width, height, -thickness,   width, height, -thickness,     // Face 1
        width, -height, thickness,   width, -height, -thickness,   width, height, -thickness,       // Face 2
        width, -height, thickness,   width, height, -thickness,  width, height, thickness,          // Face 2
        -width, -height, thickness,  width, -height, -thickness,   width, -height, thickness,       // Face 3
        -width, -height, thickness,  -width, -height, -thickness,  width, -height, -thickness,      // Face 3
        -width, -height, thickness,  width, -height, thickness,  width, height, thickness,          // Face 4
        -width, -height, thickness,  width, height, thickness,   -width, height, thickness,         // Face 4
        -width, -height, -thickness,   -width, -height, thickness,   -width, height, thickness,     // Face 5
        -width, -height, -thickness,   -width, height, thickness,  -width, height, -thickness,      // Face 5
        -width, height, thickness,   width, height, thickness,   width, height, -thickness,         // Face 6
        -width, height, thickness,   width, height, -thickness,  -width, height, -thickness         // Face 6
    };

	buildCubeDataInModel(vertices_l,model);

	loadTextures(texture_p,normalTexture_p,bumpTexture_p,model);

	computeTangents(model);

	return model;
}

void DrawableFactory::computeTangents(DrawableModel &model) {

	model.tangents = new float[model.sizeVertices];
	model.bitangents = new float[model.sizeVertices];
	for ( size_t i = 0 ; i < model.sizeVertices ; ++ i ) {
        model.tangents[i] = 0;
        model.bitangents[i] = 0;
	}
	model.sizeTangents = model.sizeVertices;
	model.sizeBitangents = model.sizeVertices;

    for ( int i=0; i<model.sizeIbo; i+=3){

        // Shortcuts for vertices
        glm::vec3 v0 = glm::vec3(model.vertices[model.ibo[i]*3],model.vertices[model.ibo[i]*3+1],model.vertices[model.ibo[i]*3+2]);
        glm::vec3 v1 = glm::vec3(model.vertices[model.ibo[i+1]*3],model.vertices[model.ibo[i+1]*3+1],model.vertices[model.ibo[i+1]*3+2]);
        glm::vec3 v2 = glm::vec3(model.vertices[model.ibo[i+2]*3],model.vertices[model.ibo[i+2]*3+1],model.vertices[model.ibo[i+2]*3+2]);

        // Shortcuts for UVs
        glm::vec2 uv0 = glm::vec2(model.textures[model.ibo[i]*3],model.textures[model.ibo[i]*3+1]);
        glm::vec2 uv1 = glm::vec2(model.textures[model.ibo[i+1]*3],model.textures[model.ibo[i+1]*3+1]);
        glm::vec2 uv2 = glm::vec2(model.textures[model.ibo[i+2]*3],model.textures[model.ibo[i+2]*3+1]);

        // Edges of the triangle : postion delta
        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;
		float r = 0;
		if ( deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x > 0.00001f || deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x < -0.00001f )
			r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

		// Set the same tangent for all three vertices of the triangle.
		model.tangents[model.ibo[i]*3] += tangent.x;
		model.tangents[model.ibo[i]*3+1] += tangent.y;
		model.tangents[model.ibo[i]*3+2] += tangent.z;
		model.tangents[model.ibo[i+1]*3] += tangent.x;
		model.tangents[model.ibo[i+1]*3+1] += tangent.y;
		model.tangents[model.ibo[i+1]*3+2] += tangent.z;
		model.tangents[model.ibo[i+2]*3] += tangent.x;
		model.tangents[model.ibo[i+2]*3+1] += tangent.y;
		model.tangents[model.ibo[i+2]*3+2] += tangent.z;

		model.bitangents[model.ibo[i]*3] += bitangent.x;
		model.bitangents[model.ibo[i]*3+1] += bitangent.y;
		model.bitangents[model.ibo[i]*3+2] += bitangent.z;
		model.bitangents[model.ibo[i+1]*3] += bitangent.x;
		model.bitangents[model.ibo[i+1]*3+1] += bitangent.y;
		model.bitangents[model.ibo[i+1]*3+2] += bitangent.z;
		model.bitangents[model.ibo[i+2]*3] += bitangent.x;
		model.bitangents[model.ibo[i+2]*3+1] += bitangent.y;
		model.bitangents[model.ibo[i+2]*3+2] += bitangent.z;
    }
}

void DrawableFactory::buildCubeDataInModel(float* vertices_p, DrawableModel &model) {
    float* textures_l = new float[96]{
        0,0.5,    1,0,    1,0.5,  // Face 1
        0,0.5,    0,0,    1,0,    // Face 1
        1,1,      0,1,    0,0.5,  // Face 2
        1,1,      0,0.5,  1,0.5,  // Face 2
        0,0.5,    1,1,    1,0.5,  // Face 3
        0,0.5,    0,1,    1,0.5,  // Face 3
        0,0.5,    1,0.5,  1,0,    // Face 4
        0,0.5,    1,0,    0,0,    // Face 4
        0,1,      1,1,    1,0.5,  // Face 5
        0,1,      1,0.5,  0,0.5,  // Face 5
        0,0.5,    1,0.5,  1,1,    // Face 6
        0,0.5,    1,1,    0,1    // Face 6
    };


    float* normals_l = new float[144]{
        -1, -1, -1,  1, 1, -1,  1, -1, -1,       // Face 1
        -1, -1, -1,   -1, 1, -1,   1, 1, -1,     // Face 1
        1, -1, 1,   1, -1, -1,   1, 1, -1,       // Face 2
        1, -1, 1,   1, 1, -1,  1, 1, 1,          // Face 2
        -1, -1, 1,  1, -1, -1,   1, -1, 1,       // Face 3
        -1, -1, 1,  -1, -1, -1,  1, -1, -1,      // Face 3
        -1, -1, 1,  1, -1, 1,  1, 1, 1,          // Face 4
        -1, -1, 1,  1, 1, 1,   -1, 1, 1,         // Face 4
        -1, -1, -1,   -1, -1, 1,   -1, 1, 1,     // Face 5
        -1, -1, -1,   -1, 1, 1,  -1, 1, -1,      // Face 5
        -1, 1, 1,   1, 1, 1,   1, 1, -1,         // Face 6
        -1, 1, 1,   1, 1, -1,  -1, 1, -1         // Face 6
    };

    factorizeDataInModel(48,vertices_p,normals_l,textures_l,model);
}

void DrawableFactory::loadTextures(std::string texture_p, std::string normalTexture_p, std::string bumpTexture_p, DrawableModel &model) {
	model.texture = new Texture(texture_p);
	model.texture->load();
	if ( normalTexture_p.size() > 0 ) {
        model.normalTexture = new Texture(normalTexture_p);
        model.normalTexture->load();
	}
	if ( bumpTexture_p.size() > 0 ) {
        model.bumpTexture = new Texture(bumpTexture_p);
        model.bumpTexture->load();
	}
}

float* vec3ToFloatArray(std::vector<glm::vec3>& vector) {
    float* array = new float[vector.size()*3];
    for ( size_t i = 0 ; i < vector.size() ; ++ i ) {
        array[i*3+0] = vector[i].x;
        array[i*3+1] = vector[i].y;
        array[i*3+2] = vector[i].z;
    }
    return array;
}

float* vec2ToFloatArray(std::vector<glm::vec2>& vector) {
    float* array = new float[vector.size()*2];
    for ( size_t i = 0 ; i < vector.size() ; ++ i ) {
        array[i*2+0] = vector[i].x;
        array[i*2+1] = vector[i].y;
    }
    return array;
}

unsigned int* vecToIntArray(std::vector<unsigned int>& vector) {
    unsigned int* array = new unsigned int[vector.size()];
    for ( size_t i = 0 ; i < vector.size() ; ++ i ) {
        array[i] = vector[i];
    }
    return array;
}

void DrawableFactory::factorizeDataInModel(size_t size_p, float* vertices_p, float* normals_p, float* textures_p, DrawableModel &model) {

    std::vector<glm::vec3> vertices_l;
    std::vector<glm::vec3> normals_l;
    std::vector<glm::vec2> textures_l;
    std::vector<unsigned int> ibo_l;

    for ( size_t i = 0 ; i < size_p ; ++ i ) {
        bool found_l = false;
        glm::vec3 vertex(vertices_p[i*3+0],vertices_p[i*3+1],vertices_p[i*3+2]);
        glm::vec3 normal(normals_p[i*3+0],normals_p[i*3+1],normals_p[i*3+2]);
        glm::vec2 texture(textures_p[i*2+0],textures_p[i*2+1]);

        for ( size_t j = 0 ; j < vertices_l.size() ; ++ j ) {
            if ( vertices_l[j] == vertex && normals_l[j] == normal && textures_l[j] == texture ) {
                found_l = true;
                ibo_l.push_back(j);
            }
        }
        // If vertices did not existed before we create it
        if ( !found_l ) {
            ibo_l.push_back(vertices_l.size());
            vertices_l.push_back(vertex);
            normals_l.push_back(glm::normalize(normal));
            textures_l.push_back(texture);
        }
    }

    // Build final data
	// Vertices
	model.sizeVertices = vertices_l.size()*3;
	model.vertices = vec3ToFloatArray(vertices_l);

    model.sizeTexture = textures_l.size()*2;
    model.textures = vec2ToFloatArray(textures_l);

    model.sizeNormals = normals_l.size()*3;
    model.normals = vec3ToFloatArray(normals_l);


    model.sizeIbo = ibo_l.size();
    model.ibo = vecToIntArray(ibo_l);
}


