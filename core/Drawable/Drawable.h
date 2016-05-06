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
		Drawable(DrawableModel inModel);
		Drawable(DrawableModel inModel, std::string const vertexShader, std::string const fragmentShader);
		/** Default destructor */
		virtual ~Drawable();

		virtual bool load();

		virtual bool draw(glm::mat4 modelview, glm::mat4 projection);

	protected:
		DrawableModel model;

		Shader shader;

		int sizeOfVerticesBytes;
		int sizeOfColorsBytes;
		int sizeOfIboBytes;
		int sizeOfTextureBytes;
		GLuint vboId;
		GLuint iboId;

	private:
		void clear();
};
