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
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)),
	sizeOfNormalsBytes(inModel->sizeNormals*sizeof(float)) {

}

Drawable::Drawable(DrawableModel* inModel, Shader* inShader) :
	model(inModel),
	shader(inShader),
	vboId(0),
	iboId(0),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)),
	sizeOfColorsBytes(inModel->sizeColors*sizeof(float)),
	sizeOfIboBytes(inModel->sizeIbo*sizeof(unsigned int)),
	sizeOfTextureBytes(inModel->sizeTexture*sizeof(float)),
	sizeOfNormalsBytes(inModel->sizeNormals*sizeof(float)) {

}

Drawable::~Drawable() {
	clear();
}

bool Drawable::load() {
	clear();

	glGenBuffers(1,&vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

		if ( model->textures != NULL ) {
			glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes + sizeOfTextureBytes, 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfNormalsBytes, sizeOfTextureBytes, model->textures);
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
}

bool Drawable::draw(glm::mat4 view, glm::mat4 modelMat, glm::mat4 projection) {

    glUseProgram(shader->getProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		if ( sizeOfNormalsBytes > 0 ) {
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes));
			glEnableVertexAttribArray(2);
		}
		if ( model->textures != NULL ) {
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes));
			glEnableVertexAttribArray(1);
			glBindTexture(GL_TEXTURE_2D, model->texture->getId());
		} else {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes + sizeOfNormalsBytes));
			glEnableVertexAttribArray(1);
		}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "mvp"), 1, GL_FALSE, value_ptr(projection*view*modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "model"), 1, GL_FALSE, value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

	glDrawElements(GL_TRIANGLES, model->sizeIbo, GL_UNSIGNED_INT, (GLvoid*)0);


	if ( model->textures != NULL ) {
		glDisableVertexAttribArray(1);
	} else {
		glDisableVertexAttribArray(1);
	}
	if ( sizeOfNormalsBytes > 0 ) {
		glDisableVertexAttribArray(2);
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
