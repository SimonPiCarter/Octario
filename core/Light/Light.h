#pragma once

#include <glm/glm.hpp>

#include "../Movable/Movable.h"

class Light : public Movable
{
	public:
		/** Default constructor */
		Light();
		/** Default destructor */
		virtual ~Light();

		virtual void translate(glm::vec3 vector);
		virtual void translate (float x, float y, float z);

		virtual void rotate(glm::vec3 axis, float angle);

		glm::vec4 getPosition();
	protected:

	private:
		glm::vec4 position;

};

