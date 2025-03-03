#pragma once
#include <unordered_map>
#include <string>

#define v_Start "@vstart"
#define v_End "@vend"

#include <d3d11.h>

#define VAR(parsedVar) v_Start + parsedVar + v_End 
 
#define SHADER_IMAGE_COUNT 15
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
		std::string GetOrRegisterImage(const ShaderSource aSource, TextureResource* aTexture, std::string aUV);

		std::string GenerateVariables();
		std::vector<TextureResource*> GetImages() { return myImages; }
	private:
		std::vector<std::string >myVariables;

		int myUniqueVariableId = 0;
		std::unordered_map<ShaderSource, std::string> myRegisteredVariables;
		std::unordered_map<ShaderSource, std::string> myRegisteredVariablesNames;

		int myUniqueImageId = 1;
		std::vector<TextureResource*> myImages;
	};
}

