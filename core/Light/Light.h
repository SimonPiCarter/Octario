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

		glm::vec4 getPosition();
	protected:

	private:
		glm::vec4 position;
};

