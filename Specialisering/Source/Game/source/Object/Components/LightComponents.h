#pragma once
#include "../Component.h"
namespace Tga
{
	class PointLight;
}

namespace Tga
{
	class PointLightComponent : public Component
	{
	public:
		PointLightComponent(GameObject* aObject);
		~PointLightComponent();

		Tga::Component* Clone(GameObject*) override;

		void Update(float aDeltaTime) override;

		void ImGuiAccess() override;

		NM::json SaveData() const override;
		void LoadData(NM::json aJsonData)override;

		void Enable() override;
		void Disable() override;

		PointLight* myPointLight = nullptr;
	};
}