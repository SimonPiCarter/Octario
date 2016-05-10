#pragma once

#include <glm/glm.hpp>

#include "../Tools/Singleton.h"
#include "DrawableModel.h"

class DrawableFactory : public Singleton<DrawableFactory>
{
	friend class Singleton<DrawableFactory>;

	public:
		DrawableModel createCubeModel(float size, float r, float g, float b);
		DrawableModel createCubeModel(float size, glm::vec3 color1,
											glm::vec3 color2,
											glm::vec3 color3,
											glm::vec3 color4,
											glm::vec3 color5,
											glm::vec3 color6);

		DrawableModel createCubeSampleTextureModel(float size);

		DrawableModel createPlaneModel(float width, float height, float thickness, float r, float g, float b);

	protected:
		DrawableFactory() {}
		virtual ~DrawableFactory() {}
	private:
		void computeTangents(DrawableModel &model);
};
