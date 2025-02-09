#include "ScriptManager.h"

#include "Script.h"
#include "ScriptJson.h"
#include "ScriptStringRegistry.h"

#include <tge/settings/settings.h>
#include <fstream>

using namespace Tga;

std::unordered_map<std::string_view, ScriptManager::InternalScriptData> ScriptManager::myLoadedScripts;

std::shared_ptr<const Script> ScriptManager::GetScript(std::string_view aName)
{
	if (!GetEditableScript(aName))
		return nullptr;

	ScriptManager::InternalScriptData& data = myLoadedScripts[aName];

	int sequenceNumber = data.script->GetSequenceNumber();
	if (data.latestSnapshotSequenceNumber == sequenceNumber)
		return data.latestSnapshot;

	std::shared_ptr<Script> newSnapshot = std::make_shared<Script>();
	ScriptJson json;
	data.script->WriteToJson(json);
	newSnapshot->LoadFromJson(json);
	newSnapshot->SetSequenceNumber(data.script->GetSequenceNumber());

	data.latestSnapshotSequenceNumber = sequenceNumber;
	data.latestSnapshot = newSnapshot;

	return newSnapshot;
}

Script* ScriptManager::GetEditableScript(std::string_view aName)
{
	auto it = myLoadedScripts.find(aName);
	if (it != myLoadedScripts.end())
		return it->second.script.get();

	//std::string path = Tga::Settings::ResolveGameAssetPath(("scripts/" + std::string(aName) + ".json"));
	std::string path = std::filesystem::current_path().generic_string() + "/Scripts/" + std::string(aName) + ".json";

	std::ifstream file(path);
	if (!file.is_open())
		return false;

	ScriptJson data = { nlohmann::json::parse(file) };
	file.close();

	ScriptStringId nameStringId = ScriptStringRegistry::RegisterOrGetString(aName);
	std::string_view nameStringView = ScriptStringRegistry::GetStringFromStringId(nameStringId);

	myLoadedScripts.insert({ nameStringView, InternalScriptData{std::make_unique<Script>(), nullptr, -1} });
	Script& script = *myLoadedScripts[nameStringView].script;

	script.LoadFromJson(data);

	return &script;
}

void ScriptManager::AddEditableScript(std::string_view name, std::unique_ptr<Script>&& script)
{
	ScriptStringId nameStringId = ScriptStringRegistry::RegisterOrGetString(name);
	std::string_view nameStringView = ScriptStringRegistry::GetStringFromStringId(nameStringId);

	myLoadedScripts.insert({ nameStringView, InternalScriptData{std::move(script), nullptr, -1} });
}