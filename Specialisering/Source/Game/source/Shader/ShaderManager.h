#pragma once
#include <string>
#include <unordered_map>
namespace Tga
{
	class ModelShader;
	class ScriptRuntimeInstance;
}

namespace Tga
{
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

		
		void Init();

		void Recompile();
		ModelShader* GetShader(std::string aName);
		//void AddShader();
		//void GetShader();

		// Scripts
		// ModelShaders
		// 
		//std::unordered_map<std::string, int> myNameToIndexes;
		const std::unordered_map<std::string, ModelShader*>& GetShaderList() { return myShaders; }

		std::unordered_map<std::string, ScriptRuntimeInstance*> myScripts;
		std::unordered_map<std::string, ModelShader*> myShaders;
	};
}