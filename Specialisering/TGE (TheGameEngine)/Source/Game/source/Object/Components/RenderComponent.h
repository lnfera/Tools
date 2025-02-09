#pragma once
#include "../Component.h"
namespace Tga
{
	class ModelInstance;
}

namespace Tga
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* aObject);
		~RenderComponent();

		Tga::Component* Clone(GameObject*) override;

		void Update(float aDeltaTime) override;

		Tga::ModelInstance* myModelInstance = nullptr;
	};
}