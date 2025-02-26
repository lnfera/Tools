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
		//Shaders are assigned withing the update function
		compileContext.compilingScriptName = pair.first;
		compileContext.compilingModelShader = myShaders[pair.first];
		myScripts[pair.first]->Update(compileContext);
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
