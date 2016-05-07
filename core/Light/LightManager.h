#pragma once

#include <string>
#include <map>

#include "Light.h"

#include "../Tools/Singleton.h"

class LightManager : public Singleton<LightManager>
{
	friend class Singleton<LightManager>;

	public:
		void addPointLight(std::string name, Light* light);
		Light* removePointLight(std::string name);

		void updatePointLightArray();

		int getPointLightCount();
		float* getPointLightArray();
	protected:
		LightManager() : pointLightsPos(NULL) {}
		virtual ~LightManager() {
			if ( pointLightsPos != NULL ) {
				delete[] pointLightsPos;
			}
		}

		float* pointLightsPos;
		std::map<std::string, Light*> pointLightPosByName;
	private:
};
