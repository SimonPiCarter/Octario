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

		virtual void translate(glm::vec3 vector);
		virtual void translate (float x, float y, float z);

		virtual void rotate(glm::vec3 axis, float angle);

	protected:
		glm::mat4 transformation;
	private:
};
