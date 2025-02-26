#include "ScriptCommon.h"
#include "ScriptJson.h"
#include "ScriptStringRegistry.h"
void Tga::ScriptLinkData::LoadFromJson(ScriptLinkDataType aType, const ScriptJson& aJsonData)
{
	if (!aJsonData.json.is_null())
	{
		switch (aType)
		{
		case ScriptLinkDataType::Bool:
			data = aJsonData.json.get<bool>();
			break;

		case ScriptLinkDataType::Int:
			data = aJsonData.json.get<int>();
			break;

		case ScriptLinkDataType::Float:
			data = aJsonData.json.get<float>();
			break;
		case ScriptLinkDataType::String:
		{
			std::string string = aJsonData.json.get<std::string>();
			data = Tga::ScriptStringRegistry::RegisterOrGetString(string);
			break;
		}
		case ScriptLinkDataType::Float4:
		{
			Vector4f vec;
			vec.x = aJsonData.json["x"];
			vec.y = aJsonData.json["y"];
			vec.z = aJsonData.json["z"];
			vec.w = aJsonData.json["w"];

			data = vec;
			break;
		}
		//case ScriptLinkDataType::NewDataType:

		//	break;
		}
	}
}

void Tga::ScriptLinkData::WriteToJson(ScriptLinkDataType aType, ScriptJson& aJsonData) const
{
	if (!std::holds_alternative<std::monostate>(data))
	{
		switch (aType)
		{
		case ScriptLinkDataType::Bool:
			aJsonData.json = std::get<bool>(data);
			break;

		case ScriptLinkDataType::Int:
			aJsonData.json = std::get<int>(data);
			break;

		case ScriptLinkDataType::Float:
			aJsonData.json = std::get<float>(data);
			break;

		case ScriptLinkDataType::String:
		{
			ScriptStringId stringId = std::get<ScriptStringId>(data);
			aJsonData.json = ScriptStringRegistry::GetStringFromStringId(stringId).data();
			break;
		}
		case ScriptLinkDataType::Float4:
		{
			aJsonData.json["x"] = std::get<float>(data);
			aJsonData.json["y"] = std::get<float>(data);
			aJsonData.json["z"] = std::get<float>(data);
			aJsonData.json["w"] = std::get<float>(data);
			break;
		}
		//case ScriptLinkDataType::NewDataType:
			//	break;
		}
	}
}
