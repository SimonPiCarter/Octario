#pragma once

#include <string>
#include "../Shader/Shader.h"
#include "DrawableModel.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class DebugDrawable
{
	public:
		/** Default constructor */
		DebugDrawable(DrawableModel* inModel);
		/** Default destructor */
		virtual ~DebugDrawable();

		virtual bool load();

		virtual bool draw(const Shader& inShader);

	protected:
		DrawableModel* model;

		float* m_vertices;
		float* m_colors;
		float* m_ibos;
		size_t m_sizeIbo;

		int sizeOfVerticesBytes;
		int sizeOfColorsBytes;
		int sizeOfIboBytes;
		GLuint vboId;
		GLuint iboId;

	private:
		void clear();
};
