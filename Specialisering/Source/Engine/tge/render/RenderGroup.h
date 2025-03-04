#pragma once
#include <vector>

namespace Tga
{
	class Model;
	class ModelShader;
	class ModelInstance;
}

namespace Tga
{
	struct RenderGroup
	{
		Model* model = nullptr;
		ModelShader* modelShader = nullptr;
		uint32_t renderFlags;
		std::vector<ModelInstance*> modelInstances;
	};
}