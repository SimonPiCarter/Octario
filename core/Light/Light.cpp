#include "Light.h"

Light::Light() : position(0,0,0,1)
{
	//ctor
}

Light::~Light()
{
	//dtor
}

glm::vec4 Light::getPosition() {
	return getMatrix()*position;
}
