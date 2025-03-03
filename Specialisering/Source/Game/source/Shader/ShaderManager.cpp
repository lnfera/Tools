#include "ShaderManager.h"
#include <tge/engine.h>
#include <tge/shaders/ModelShader.h>
#include <Scripting/Script.h>
#include <Scripting/ScriptManager.h>
#include <Scripting/ScriptRuntimeInstance.h>
#include <Scripting/ScriptStringRegistry.h>
#include <Scripting/Context/ScriptUpdateContext.h>
Tga::ShaderManager::ShaderManager()
{
	Tga::ScriptManager::Init();
}

Tga::ShaderManager::~ShaderManager()
{
	for (auto& pair : myShaders)
	{
		delete pair.second;
	}
	for (auto& pair : myScripts)
	{
		delete pair.second;
	}
}

void Tga::ShaderManager::Init()
{
	// Load All Shader Scripts
	{

	}

	Recompile();
}
void Tga::ShaderManager::Recompile()
{
	std::vector<std::string_view> scriptNameList = Tga::ScriptManager::GetScriptNameList();

	//Create Script Instances
	for (auto& scriptStrView : scriptNameList)
	{
		std::string scriptName = scriptStrView.data();

		auto script = Tga::ScriptManager::GetScript(scriptName);

		if (script->IsShaderScript() == false)
		{
			continue;
		}


		if (myScripts[scriptName] != nullptr)
		{
			delete myScripts[scriptName];
		}

		myScripts[scriptName] = new ScriptRuntimeInstance(script);
		myScripts[scriptName]->Init();
	}

	//Create Modelshaders
	for (auto& pair : myScripts)
	{
		ScriptRuntimeInstance* instance = pair.second;
		std::string scriptname = instance->GetScript().GetScriptName();
		if (myShaders[scriptname] == nullptr)
		{
			myShaders[scriptname] = new ModelShader(TGE_I());
		}
		myShaders[scriptname]->Init();
	}

	//Compile all scripts
	Tga::ShaderCompileContext compileContext;
	for (auto& pair : myScripts)
	{

		std::string hlslCode = "";

		//Shaders are assigned withing the update function
		compileContext.compilingScriptName = pair.first;
		compileContext.compilingModelShader = myShaders[pair.first];
		compileContext.hlslCode = &hlslCode;

		myScripts[pair.first]->Update(compileContext);
		
		myHLSLCode[pair.first] = hlslCode;
	}
}

void Tga::ShaderManager::ExportHLSLCodeToConsole(const std::string& aShaderName)
{
	if (myHLSLCode.find(aShaderName) != myHLSLCode.end())
	{
		std::cout << myHLSLCode[aShaderName] << '\n';
	}
}
void Tga::ShaderManager::ExportHLSLCodeToClipBoard(const std::string& aShaderName)
{
	if (myHLSLCode.find(aShaderName) != myHLSLCode.end())
	{
		if (myHLSLCode[aShaderName].empty())
			return;

		// Allocate global memory for the clipboard
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, myHLSLCode[aShaderName].size() + 1);
		if (!hMem)
			return;

		// Lock the memory and copy the string
		memcpy(GlobalLock(hMem), myHLSLCode[aShaderName].c_str(), myHLSLCode[aShaderName].size() + 1);
		GlobalUnlock(hMem);

		// Open the clipboard and empty it
		if (OpenClipboard(nullptr))
		{
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
		else
		{
			GlobalFree(hMem);
		}
	}
}

#include <fstream>
#include <filesystem>
void Tga::ShaderManager::ExportHLSLToFile(const std::string& aShaderName)
{
	if (myHLSLCode.find(aShaderName) != myHLSLCode.end())
	{
		std::string dirPath = std::filesystem::current_path().generic_string() + "/Exports/";
		
		if (!std::filesystem::exists(dirPath))
		{
			std::filesystem::create_directories(dirPath);
		}
		
		std::string p = dirPath + aShaderName + ".hlsl";

		std::ofstream file;
		file.open(p);

		if (file.is_open())
		{
			file << myHLSLCode[aShaderName];
		}
		file.close();
	}
}


Tga::ModelShader* Tga::ShaderManager::GetShader(std::string aName)
{
	if (myShaders.find(aName) != myShaders.end()) 
	{
		return myShaders[aName];
	}

	return nullptr;
}
