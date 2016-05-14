#pragma once

#include <glm/glm.hpp>

#include "../Tools/Singleton.h"
#include "DrawableModel.h"

class DrawableFactory : public Singleton<DrawableFactory>
{
	friend class Singleton<DrawableFactory>;

	public:
		DrawableModel createCubeSampleTextureModel(float size);

		DrawableModel createPlaneModel(float width, float height, float thickness, float r, float g, float b);

	protected:
		DrawableFactory() {}
		virtual ~DrawableFactory() {}
	private:
		void computeTangents(DrawableModel &model);

		void buildCubeDataInModel(float* vertices_p, DrawableModel &model);

		void factorizeDataInModel(size_t size, float* vertices_p, float* normals_p, float* textures_p, DrawableModel &model);
};
