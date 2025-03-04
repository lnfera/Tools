#pragma once
#include "../Component.h"
namespace Tga
{
	class ModelInstance;
	class ModelShader;
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

		void ImGuiAccess() override;

		NM::json SaveData() const override;
		void LoadData(NM::json aJsonData)override;

		void Enable() override;
		void Disable() override;

		Tga::ModelInstance* myModelInstance = nullptr;
		Tga::ModelShader* myShader = nullptr;

		std::string myShaderName = "none";
		std::string myTexturePaths[4] = { "none","none","none", "none"};
	};
}