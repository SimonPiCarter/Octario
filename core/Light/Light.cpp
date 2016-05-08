#include "Light.h"

Light::Light() : position(0,0,0,1)
{
	//ctor
}

Light::~Light()
{
	//dtor
}


void Light::translate(glm::vec3 vector) {
	Movable::translate(vector);
	position = getMatrix()*glm::vec4(0,0,0,1);
}
void Light::translate (float x, float y, float z) {
	Movable::translate(x,y,z);
	position = getMatrix()*glm::vec4(0,0,0,1);
}

void Light::rotate(glm::vec3 axis, float angle) {
	Movable::rotate(axis,angle);
	position = getMatrix()*glm::vec4(0,0,0,1);
}

glm::vec4 Light::getPosition() {
	return position;
}
