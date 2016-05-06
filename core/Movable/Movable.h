#pragma once

#include <glm/glm.hpp>

class Movable
{
	public:
		/** Default constructor */
		Movable();
		/** Default destructor */
		virtual ~Movable();

		glm::mat4 getMatrix();

		void translate(glm::vec3 vector);
		void translate (float x, float y, float z);

		void rotate(glm::vec3 axis, float angle);

	protected:
		glm::mat4 transformation;
	private:
};
