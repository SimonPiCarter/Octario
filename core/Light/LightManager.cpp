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
	pointLightsProp = new float[getPointLightCount()*4];
	int index = 0;

	for ( std::map<std::string,Light*>::iterator it = pointLightPosByName.begin(); it != pointLightPosByName.end() ; ++it ) {
		glm::vec4 pos = it->second->getPosition();
		glm::vec4 prop = it->second->getProperties();
		pointLightsProp[index] = prop.x;	pointLightsPos[index++] = pos.x;
		pointLightsProp[index] = prop.y;	pointLightsPos[index++] = pos.y;
		pointLightsProp[index] = prop.z;	pointLightsPos[index++] = pos.z;
		pointLightsProp[index] = prop.w;	pointLightsPos[index++] = pos.w;
	}
}

int LightManager::getPointLightCount() {
	return pointLightPosByName.size();
}

float* LightManager::getPointLightsPos() {
	return pointLightsPos;
}

float* LightManager::getPointLightsProp() {
	return pointLightsProp;
}
