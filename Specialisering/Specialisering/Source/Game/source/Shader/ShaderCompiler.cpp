#include "ShaderCompiler.h"
#include <iostream>
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
	for (auto& data : myConnections)
	{

		std::cout << "Reciever: " << data.first << " Source: " << data.second;

		if (data.second == nullptr)
		{
			std::cout << "\033[32m LEAF: \033[0m";
		}

		std::cout << '\n';

	}
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
	std::string uniqueVarName = "var" + std::to_string(++myUniqueVariableId) + "e ";
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



std::string Tga::ShaderParseCompiler::GenerateVariables()
{
	std::string output;

	for (int i = 0; i < myVariables.size(); i++)
	{
		int itr = (int)myVariables[i].find_first_of("e") + 1;

		myVariables[i].insert(myVariables[i].begin() + itr, '=');

		output += myVariables[i] + ";//\n";
	}

	//for (auto& variable : myRegisteredVariables)
	//{
	//	int itr = (int)variable.second.find_first_of("e") + 1;

	//	variable.second.insert(variable.second.begin() + itr, '=');

	//	output += variable.second + ";//\n";
	//}

	return output;
}
