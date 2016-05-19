#include "Camera.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

Camera::Camera(): m_transformation(), m_up(0,1,0), m_right(-1,0,0), m_front(0,0,1), m_position(0,0,0), m_yaw(0), m_pitch(0) {
    //ctor
}

Camera::~Camera(){
    //dtor
}


void Camera::translate(glm::vec3 vector) {
    translate(vector.x,vector.y,vector.z);
}

void Camera::translate (float x, float y, float z){
    m_position += x*m_right + y*m_up + z*m_front;
}

glm::vec3 vec4ToVec3(const glm::vec4 v) {
    return glm::vec3(v.x,v.y,v.z);
}

void Camera::pitch(float angle) {
    if ( m_pitch+angle < 80.f && m_pitch+angle > -80.f ) {
        m_pitch += angle;
    }
    update();
}
void Camera::yaw(float angle) {
    m_yaw += angle;
    update();
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(m_position, m_position+m_front, m_up);
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}

void Camera::update() {
    m_front.x = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

    m_up.x = -sin(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_up.y = cos(glm::radians(m_pitch));
    m_up.z = -sin(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

    m_right.x = -cos(glm::radians(m_yaw));
    m_right.y = 0;
    m_right.z = sin(glm::radians(m_yaw));
}
