#pragma once

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "../Shader/Shader.h"

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

class Drawable
{
	public:
		/** Default constructor */
		Drawable(float* inVertices, int nbVertices, float* inColors, int nbColors, unsigned int* inIbo, int nbIbo);
		Drawable(float* inVertices, int nbVertices, float* inColors, int nbColors, unsigned int* inIbo, int nbIbo,
				std::string const vertexShader, std::string const fragmentShader);
		/** Default destructor */
		virtual ~Drawable();

		virtual bool load();

		virtual bool draw(glm::mat4 modelview, glm::mat4 projection);

	protected:
		float *vertices;
		float *colors;
		unsigned int *ibo;

		Shader shader;

		int sizeOfVerticesBytes;
		int sizeOfColorsBytes;
		int sizeOfIboBytes;
		GLuint vboId;
		GLuint iboId;

	private:
		void clear();
};
