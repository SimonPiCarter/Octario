#include "Drawable.h"

using namespace glm;

Drawable::Drawable(DrawableModel* inModel) :
	model(inModel),
	shader(),
	vboId(0),
	iboId(0),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)),
	sizeOfColorsBytes(inModel->sizeColors*sizeof(float)),
	sizeOfIboBytes(inModel->sizeIbo*sizeof(unsigned int)),
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)) {

}

Drawable::Drawable(DrawableModel* inModel, Shader* inShader) :
	model(inModel),
	shader(inShader),
	vboId(0),
	iboId(0),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)),
	sizeOfColorsBytes(inModel->sizeColors*sizeof(float)),
	sizeOfIboBytes(inModel->sizeIbo*sizeof(unsigned int)),
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)) {

}

Drawable::~Drawable() {
	clear();
}

bool Drawable::load() {
	clear();

	glGenBuffers(1,&vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

		if ( model->textures != NULL ) {
			glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfTextureBytes, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes, sizeOfTextureBytes, model->textures);
		} else if ( model->colors != NULL ) {
			glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfColorsBytes, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes, sizeOfColorsBytes, model->colors);
		}
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesBytes, model->vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIboBytes, model->ibo, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Drawable::draw(mat4 modelview, mat4 projection) {

    glUseProgram(shader->getProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	if ( model->textures != NULL ) {
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes));
		glEnableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE_2D, model->texture->getId());
	} else {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes));
		glEnableVertexAttribArray(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

	glDrawElements(GL_TRIANGLES, model->sizeIbo, GL_UNSIGNED_INT, (GLvoid*)0);


	if ( model->textures != NULL ) {
		glDisableVertexAttribArray(2);
	} else {
		glDisableVertexAttribArray(1);
	}
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);
}

void Drawable::clear() {
	if ( glIsBuffer(vboId) == GL_TRUE ) {
		glDeleteBuffers(1, &vboId);
	}
	if ( glIsBuffer(iboId) == GL_TRUE ) {
		glDeleteBuffers(1, &iboId);
	}
}
