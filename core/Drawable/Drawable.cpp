#include "Drawable.h"
#include "../Light/LightManager.h"

using namespace glm;

Drawable::Drawable(DrawableModel* inModel) :
	model(inModel),
	shader(),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)),
	sizeOfColorsBytes(inModel->sizeColors*sizeof(float)),
	sizeOfIboBytes(inModel->sizeIbo*sizeof(unsigned int)),
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)),
	sizeOfNormalsBytes(inModel->sizeNormals*sizeof(float)),
	sizeOfTangentsBytes(inModel->sizeTangents*sizeof(float)),
	sizeOfBitangentsBytes(inModel->sizeBitangents*sizeof(float)),
	vboId(0),
	iboId(0) {

}

Drawable::Drawable(DrawableModel* inModel, Shader* inShader) :
	model(inModel),
	shader(inShader),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)),
	sizeOfColorsBytes(inModel->sizeColors*sizeof(float)),
	sizeOfIboBytes(inModel->sizeIbo*sizeof(unsigned int)),
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)),
	sizeOfNormalsBytes(inModel->sizeNormals*sizeof(float)),
	vboId(0),
	iboId(0) {

}

Drawable::~Drawable() {
	clear();
}

bool Drawable::load() {
	clear();

	glGenBuffers(1,&vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

		if ( model->textures != NULL ) {
			glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTangentsBytes + sizeOfBitangentsBytes + sizeOfTextureBytes, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes, sizeOfTangentsBytes, model->tangents);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTangentsBytes, sizeOfBitangentsBytes, model->bitangents);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTangentsBytes + sizeOfBitangentsBytes, sizeOfTextureBytes, model->textures);
		} else if ( model->colors != NULL ) {
			glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfColorsBytes, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes, sizeOfColorsBytes, model->colors);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesBytes, model->vertices);
		glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes, sizeOfNormalsBytes, model->normals);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIboBytes, model->ibo, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Drawable::draw() {

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		if ( sizeOfNormalsBytes > 0 ) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes));
			glEnableVertexAttribArray(1);
		}
		if ( model->textures != NULL ) {
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTangentsBytes));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTangentsBytes + sizeOfBitangentsBytes));
			glEnableVertexAttribArray(4);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model->texture->getId());
			glUniform1i(glGetUniformLocation(shader->getProgramID(), "texture"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, model->normalTexture->getId());
			glUniform1i(glGetUniformLocation(shader->getProgramID(), "normalTexture"), 1);
		} else {
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes));
			glEnableVertexAttribArray(2);
		}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	glDrawElements(GL_TRIANGLES, model->sizeIbo, GL_UNSIGNED_INT, (GLvoid*)0);


	if ( model->textures != NULL ) {
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
	} else {
		glDisableVertexAttribArray(2);
	}
	if ( sizeOfNormalsBytes > 0 ) {
		glDisableVertexAttribArray(1);
	}
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Drawable::clear() {
	if ( glIsBuffer(vboId) == GL_TRUE ) {
		glDeleteBuffers(1, &vboId);
	}
	if ( glIsBuffer(iboId) == GL_TRUE ) {
		glDeleteBuffers(1, &iboId);
	}
}
