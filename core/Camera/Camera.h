#pragma once

#include <glm/glm.hpp>

class Camera
{
    public:
        Camera();
        ~Camera();

		void translate(glm::vec3 vector);
		void translate (float x, float y, float z);

		void pitch(float angle);
		void yaw(float angle);

		glm::mat4 getView() const;
		glm::vec3 getPosition() const;

	protected:
		glm::mat4 m_transformation;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_front;
		glm::vec3 m_position;
		float m_yaw;
		float m_pitch;
    private:
        void update();
};
