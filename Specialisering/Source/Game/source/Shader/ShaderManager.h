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
		
		void ExportHLSLCodeToConsole(const std::string& aShaderName);
		void ExportHLSLCodeToClipBoard(const std::string& aShaderName);
		void ExportHLSLToFile(const std::string& aShaderName);

		const std::unordered_map<std::string, ModelShader*>& GetShaderList() { return myShaders; }

	private:
		std::unordered_map<std::string, ModelShader*> myShaders;
		std::unordered_map<std::string, std::string> myHLSLCode;
		std::unordered_map<std::string, ScriptRuntimeInstance*> myScripts;
	};
}