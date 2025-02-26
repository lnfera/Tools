#include "ShaderCompiler.h"
#include <iostream>
#include <cassert>
// Regular expression to match variables between @vstart and @vend

Tga::ShaderParseCompiler::ShaderParseCompiler()
{
}

Tga::ShaderParseCompiler::~ShaderParseCompiler()
{
}

void Tga::ShaderParseCompiler::PrintParsedData()
{
	std::cout << "\n";
	std::cout << "\033[32m START PRINT \033[0m\n";

	std::cout << "\033[33m Variables: \033[0m\n";

	for (auto& data : myVariables)
	{
		std::cout << /*"Source: " << data.first <<*/ " Data:" << data << '\n';
	}

	std::cout << "\033[33m Connections: \033[0m\n";

	std::cout << "\033[33m Registered Variables : \033[0m\n";

	for (auto& data : myRegisteredVariables)
	{
		std::cout << "Source: " << data.first << " Variable: " << data.second << '\n';
	}


	std::cout << "\033[31m END PRINT \033[0m\n";

	std::cout << "\n";
}

void Tga::ShaderParseCompiler::RegisterVariable(const ShaderSource aSource, const std::string& aDataType, const std::string& aValue)
{
	// should be giving something like " float var2 0.3333f
	std::string uniqueVarName = "var" + std::to_string(++myUniqueVariableId) + "z";
	std::string reg = aDataType + " " + uniqueVarName + " " + aValue;

	myVariables.push_back(reg);
	myRegisteredVariables[aSource] = reg;
	myRegisteredVariablesNames[aSource] = uniqueVarName;
}

bool Tga::ShaderParseCompiler::isVariableRegistered(const ShaderSource aSource)
{
	if (myRegisteredVariables.find(aSource) != myRegisteredVariables.end())
	{
		return true;
	}
	return false;
}

std::string Tga::ShaderParseCompiler::GetRegisteredVariableName(const ShaderSource aSource)
{
	return myRegisteredVariablesNames[aSource];
}

std::string Tga::ShaderParseCompiler::GetOrRegisterImage(const ShaderSource aSource, TextureResource* aTexture)
{
	if(myRegisteredVariables.find(aSource) == myRegisteredVariables.end())
	{
		int imgId = myUniqueImageId++;
		std::string uniqueVarName = "image" + std::to_string(imgId)+"z";
		std::string reg = "float4 " + uniqueVarName + " imageText" + std::to_string(imgId) + "z.Sample(defaultSampler, scaledUV).rgba";

		myVariables.push_back(reg);
		myRegisteredVariables[aSource] = reg;
		myRegisteredVariablesNames[aSource] = uniqueVarName;
		myImages.push_back(aTexture);
		return uniqueVarName;

	}
	return myRegisteredVariablesNames[aSource];
}



std::string Tga::ShaderParseCompiler::GenerateVariables()
{
	std::string output;

	for (int i = 0; i < myVariables.size(); i++)
	{
		int itr = (int)myVariables[i].find_first_of("z") + 1;

		myVariables[i].insert(myVariables[i].begin() + itr, '=');

		output += myVariables[i] + ";\n";
	}

	return output;
}
