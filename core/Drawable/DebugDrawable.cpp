#include "DebugDrawable.h"
#include "../Light/LightManager.h"

using namespace glm;

DebugDrawable::DebugDrawable(DrawableModel* inModel) :
	model(inModel),
	sizeOfVerticesBytes(inModel->sizeVertices*sizeof(float)*6),
	sizeOfColorsBytes(inModel->sizeVertices*sizeof(float)*6),
	sizeOfIboBytes(inModel->sizeVertices*sizeof(unsigned int)*6),
	vboId(0),
	iboId(0) {
}

DebugDrawable::~DebugDrawable() {
	clear();
}

bool DebugDrawable::load() {
	clear();

	// build data
    m_vertices = new float[model->sizeVertices*6*3];
    m_colors = new float[model->sizeVertices*6*3];
    m_ibos = new float[model->sizeVertices*6];
    m_sizeIbo = model->sizeVertices*6;
    size_t index = 0;
    size_t ibo_index = 0;
    // for each vertex
    for ( size_t i = 0 ; i < model->sizeVertices/3 ; ++ i ) {
        // build line for normal (blue)
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 0.f;        m_colors[index+1] = 0.f;        m_colors[index+2] = 1.f;
        m_vertices[index++] = model->vertices[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2];
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 0.f;        m_colors[index+1] = 0.f;        m_colors[index+2] = 1.f;
        m_vertices[index++] = model->vertices[i*3+0]+model->normals[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1]+model->normals[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2]+model->normals[i*3+2];

        // Build line for tangent
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 1.f;        m_colors[index+1] = 0.f;        m_colors[index+2] = 0.f;
        m_vertices[index++] = model->vertices[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2];
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 1.f;        m_colors[index+1] = 0.f;        m_colors[index+2] = 0.f;
        m_vertices[index++] = model->vertices[i*3+0]+model->tangents[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1]+model->tangents[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2]+model->tangents[i*3+2];

        // Build line for bitangent
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 0.f;        m_colors[index+1] = 1.f;        m_colors[index+2] = 0.f;
        m_vertices[index++] = model->vertices[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2];
        m_ibos[ibo_index] = ibo_index;        ++ibo_index;
        m_colors[index] = 0.f;        m_colors[index+1] = 1.f;        m_colors[index+2] = 0.f;
        m_vertices[index++] = model->vertices[i*3+0]+model->bitangents[i*3+0];
        m_vertices[index++] = model->vertices[i*3+1]+model->bitangents[i*3+1];
        m_vertices[index++] = model->vertices[i*3+2]+model->bitangents[i*3+2];
    }

	glGenBuffers(1,&vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

        glBufferData(GL_ARRAY_BUFFER, sizeOfVerticesBytes + sizeOfColorsBytes, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, sizeOfVerticesBytes, sizeOfColorsBytes, m_colors);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVerticesBytes, m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIboBytes, m_ibos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool DebugDrawable::draw(const Shader& inShader) {

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfVerticesBytes));
        glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	glDrawElements(GL_LINES, m_sizeIbo, GL_UNSIGNED_INT, (GLvoid*)0);

    glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void DebugDrawable::clear() {
	if ( glIsBuffer(vboId) == GL_TRUE ) {
		glDeleteBuffers(1, &vboId);
	}
	if ( glIsBuffer(iboId) == GL_TRUE ) {
		glDeleteBuffers(1, &iboId);
	}
}
