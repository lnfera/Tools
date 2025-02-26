#pragma once
namespace Tga
{

	class ModelShader;
	class GameObject;
	struct ScriptUpdateContext 
	{
		float deltaTime = -1.f;
	};

	struct GameUpdateContext : public ScriptUpdateContext
	{
		GameObject* activeObject = nullptr;
	};

	struct ShaderCompileContext : public ScriptUpdateContext
	{
		std::string compilingScriptName = "x";
		ModelShader* compilingModelShader = nullptr;
	};

}