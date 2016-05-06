#include "Movable.h"

#include <glm/gtx/transform.hpp>

Movable::Movable()
{
	//ctor
}

Movable::~Movable()
{
	//dtor
}

glm::mat4 Movable::getMatrix() {
	return transformation;
}

void Movable::translate(glm::vec3 vector) {
	transformation = glm::translate(transformation,vector);
}
void Movable::translate (float x, float y, float z) {
	transformation = glm::translate(transformation,glm::vec3(x,y,z));
}

void Movable::rotate(glm::vec3 axis, float angle) {
	transformation = glm::rotate(transformation,angle,axis);
}
