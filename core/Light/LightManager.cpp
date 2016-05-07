#include "LightManager.h"

#include <glm/glm.hpp>

void LightManager::addPointLight(std::string name, Light* light) {
	if ( getPointLightCount() < 16 ) {
		pointLightPosByName[name] = light;
		updatePointLightArray();
	}
}
Light* LightManager::removePointLight(std::string name) {
	Light* old = pointLightPosByName[name];
	pointLightPosByName.erase(name);
	updatePointLightArray();
	return old;
}

void LightManager::updatePointLightArray() {
	if ( pointLightsPos != NULL ) {
		delete[] pointLightsPos;
	}
	pointLightsPos = new float[getPointLightCount()*4];
	int index = 0;

	for ( std::map<std::string,Light*>::iterator it = pointLightPosByName.begin(); it != pointLightPosByName.end() ; ++it ) {
		glm::vec4 vec = it->second->getPosition();
		pointLightsPos[index++] = vec.x;
		pointLightsPos[index++] = vec.y;
		pointLightsPos[index++] = vec.z;
		pointLightsPos[index++] = vec.w;
	}
}

int LightManager::getPointLightCount() {
	return pointLightPosByName.size();
}

float* LightManager::getPointLightArray() {
	return pointLightsPos;
}
