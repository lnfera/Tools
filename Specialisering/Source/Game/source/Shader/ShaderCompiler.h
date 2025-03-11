#pragma once
#include <unordered_map>
#include <string>
#include <d3d11.h>

#include <tge/shaders/ModelShader.h>
namespace Tga 
{
	class ScriptNodeBase;
	class TextureResource;
}

typedef void* ShaderSource;

namespace Tga
{
	/// <summary>
	/// Used when compiling shaders from nodes. Keeps it optimized and puts the entire HLSL string together,
	/// </summary>
	class ShaderParseCompiler
	{
	public:
		ShaderParseCompiler();
		~ShaderParseCompiler();

		void PrintParsedData();

		void RegisterVariable(const ShaderSource aSource, const std::string& aDataType,const std::string& aValue);


		bool isVariableRegistered(const ShaderSource aSource);

		std::string GetRegisteredVariableName(const ShaderSource aSource);
		std::string GetOrRegisterTexture(const ShaderSource aSource, int aSlot,std::string aUV);
		std::string GetOrRegisterImage(const ShaderSource aSource, TextureResource* aTexture, std::string aUV);

		std::string GenerateVariables();
		std::vector<TextureResource*> GetImages() { return myImages; }
	private:
		std::vector<std::string >myVariables;

		int myUniqueVariableId = 0;
		std::unordered_map<ShaderSource, std::string> myRegisteredVariables;
		std::unordered_map<ShaderSource, std::string> myRegisteredVariablesNames;

		//int myUniqueImageId = 1;
		int myUniqueImageId = REG_TEXT_OFFSET;
		std::vector<TextureResource*> myImages;
	};
}

