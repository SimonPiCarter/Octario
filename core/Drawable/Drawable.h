#pragma once

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "../Shader/Shader.h"
#include "DrawableModel.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class Drawable
{
	public:
		/** Default constructor */
		Drawable(DrawableModel* inModel);
		/** Default destructor */
		virtual ~Drawable();

		virtual bool load();

		virtual bool draw(const Shader& inShader);

	protected:
		DrawableModel* model;

		int sizeOfVerticesBytes;
		int sizeOfColorsBytes;
		int sizeOfIboBytes;
		int sizeOfTextureBytes;
		int sizeOfNormalsBytes;
		int sizeOfTangentsBytes;
		int sizeOfBitangentsBytes;
		GLuint vboId;
		GLuint iboId;

	private:
		void clear();
};
