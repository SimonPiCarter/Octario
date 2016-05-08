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
		glm::vec4 getProperties();

		void setProperties(float r, float g, float b, float power);
		void setPower(float power);
		void setColor(float r, float g, float b);
	protected:

	private:
		glm::vec4 position;

		glm::vec4 properties;
};

