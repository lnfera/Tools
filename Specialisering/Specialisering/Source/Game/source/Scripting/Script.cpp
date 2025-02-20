#include "Script.h"
#include "ScriptJson.h"

#include "ScriptStringRegistry.h"
#include "ScriptNodeTypeRegistry.h"
#include "ScriptCreationContext.h"

#include <cassert>
Tga::Script::Script() = default;
Tga::Script::~Script() = default;

using namespace Tga;

void Tga::Script::Clear()
{
	mySequenceNumber = 0;

	myNodes.clear();
	myFreeNodes.clear();

	myLinks.clear();
	myFreeLinks.clear();

	myPins.clear();
	myFreePins.clear();

	myInstanceToId.clear();
}

void Tga::Script::LoadFromJson(const ScriptJson& aData)
{
	// todo: handle unknown node types

	assert("Trying to load into a script that is not empty" && GetFirstNodeId().id == ScriptNodeId::InvalidId);
	assert("Trying to load into a script that is not empty" && GetFirstPinId().id == ScriptPinId::InvalidId);
	assert("Trying to load into a script that is not empty" && GetFirstLinkId().id == ScriptLinkId::InvalidId);

	using namespace nlohmann;

	myIsShaderScript = aData.json["isShaderScript"];

	const json& nodes = aData.json["nodes"];
	const json& pins = aData.json["pins"];
	const json& links = aData.json["links"];

	for (json::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		const json& nodeData = *it;
		ScriptNodeId id = { nodeData["id"].get<unsigned int>() };
		const json& posArray = nodeData["position"];
		Vector2 pos = { posArray[0].get<float>(), posArray[1].get<float>() };
		std::string typeName = nodeData["type"];
		std::string instanceName = nodeData["name"];
		const json& customNodeData = nodeData["customData"];

		ScriptNodeTypeId typeId = ScriptNodeTypeRegistry::GetTypeId(typeName);

		CreateNodeWithReusedId(id, typeId, ScriptNodeTypeRegistry::CreateNode(typeId), pos);
		ScriptNodeBase& node = GetNode(id);
		if (!instanceName.empty())
			SetName(id, ScriptStringRegistry::RegisterOrGetString(instanceName));

		node.LoadFromJson({ customNodeData });
	}

	for (json::const_iterator it = pins.begin(); it != pins.end(); ++it)
	{
		const json& pinData = *it;
		ScriptPinId id = { pinData["id"].get<unsigned int>() };
		ScriptNodeId nodeId = { pinData["node"].get<unsigned int>() };
		std::string name = pinData["name"];

		std::string dataTypeString = pinData["dataType"];
		ScriptLinkDataType dataType = ScriptLinkDataType::UNKNOWN;
		for (int i = 0; i < (int)ScriptLinkDataType::COUNT; i++)
		{
			if (dataTypeString == ScriptLinkDataTypeNames[i])
			{
				dataType = (ScriptLinkDataType)i;
				break;
			}
		}

		ScriptLinkData overridenValue = { std::monostate() };
		overridenValue.LoadFromJson(dataType, { pinData["value"] });

		std::string roleString = pinData["role"];
		ScriptPinRole role;
		if (roleString == "Output")
			role = ScriptPinRole::Output;
		else
			role = ScriptPinRole::Input;

		CreatePinWithReusedId(id, { role, INT_MAX, nodeId, ScriptStringRegistry::RegisterOrGetString(name), dataType, std::monostate(), overridenValue });
	}

	for (json::const_iterator it = links.begin(); it != links.end(); ++it)
	{
		const json& linkData = *it;
		ScriptLinkId id = { linkData["id"].get<unsigned int>() };
		ScriptPinId sourcePin = { linkData["sourcePin"].get<unsigned int>() };
		ScriptPinId targetPin = { linkData["targetPin"].get<unsigned int>() };

		CreateLinkWithReusedId(id, { sourcePin, targetPin });
	}

	//Tga::Engine::GetInstance()->GetPostMaster().ClearAllEvent();
	for (ScriptNodeId nodeId = GetFirstNodeId(); nodeId.id != ScriptNodeId::InvalidId; nodeId = GetNextNodeId(nodeId))
	{
		ScriptNodeBase& node = GetNode(nodeId);
		ScriptCreationContext context(*this, nodeId);

		node.Init(context);
	}

}
void Tga::Script::WriteToJson(ScriptJson& aJsonData)
{
	using namespace nlohmann;

	aJsonData.json["nodes"] = json::array();
	json& nodes = aJsonData.json["nodes"];

	for (ScriptNodeId nodeId = GetFirstNodeId(); nodeId.id != ScriptNodeId::InvalidId; nodeId = GetNextNodeId(nodeId))
	{
		const ScriptNodeBase& node = GetNode(nodeId);
		ScriptNodeTypeId type = GetType(nodeId);
		ScriptStringId name = GetName(nodeId);
		Tga::Vector2f pos = GetPosition(nodeId);

		ScriptJson nodeResult;
		node.WriteToJson(nodeResult);

		// The data that we save
		nodes.push_back(
			{
				{"id", nodeId.id},
				{"type", ScriptNodeTypeRegistry::GetNodeTypeShortName(type).data()},
				{"name", name.id != ScriptStringId::InvalidId ? ScriptStringRegistry::GetStringFromStringId(name).data() : ""},
				{"customData", nodeResult.json},
				{"position", {pos.x, pos.y} }
			}
		);
	}

	aJsonData.json["pins"] = json::array();
	json& pins = aJsonData.json["pins"];

	for (ScriptPinId pinId = GetFirstPinId(); pinId.id != ScriptPinId::InvalidId; pinId = GetNextPinId(pinId))
	{
		const ScriptPin& pin = GetPin(pinId);

		// Skip unconnected pins where default value hasn't been overriden. They will be recreated when the script is loaded.
		size_t connectedLinksCount;
		GetConnectedLinks(pinId, connectedLinksCount);
		if (connectedLinksCount == 0 && std::holds_alternative<std::monostate>(pin.overridenValue.data))
			continue;

		ScriptJson overridenValue = {};
		pin.overridenValue.WriteToJson(pin.dataType, overridenValue);

		pins.push_back(
			{
				{"id", pinId.id},
				{"node", pin.node.id},
				{"name", ScriptStringRegistry::GetStringFromStringId(pin.name).data()},
				{"role", pin.role == ScriptPinRole::Input ? "Input" : "Output" },
				{"value", overridenValue.json},
				{"dataType", ScriptLinkDataTypeNames[(int)pin.dataType]}
			});

	}

	aJsonData.json["links"] = json::array();
	json& links = aJsonData.json["links"];

	for (ScriptLinkId linkId = GetFirstLinkId(); linkId.id != ScriptLinkId::InvalidId; linkId = GetNextLinkId(linkId))
	{

		const ScriptLink& link = GetLink(linkId);

		links.push_back(
			{
				{"id", linkId.id},
				{"sourcePin", link.sourcePinId.id},
				{"targetPin", link.targetPinId.id},
			});
	}


	aJsonData.json["isShaderScript"] = myIsShaderScript;
}

Tga::ScriptNodeId Tga::Script::CreateNode(Tga::ScriptNodeTypeId aTypeId)
{
	UpdateSequenceNumber();

	if (myFreeNodes.empty())
	{
		ScriptNodeId id = { (unsigned int)myNodes.size() };
		myNodes.push_back({ false, aTypeId, ScriptStringId {ScriptStringId::InvalidId} });
		return id;
	}
	ScriptNodeId id = myFreeNodes.back();
	myFreeNodes.pop_back();

	myNodes[id.id] = { false, aTypeId, ScriptStringId {ScriptStringId::InvalidId} };
	return id;
}
Tga::ScriptLinkId Tga::Script::CreateLink()
{
	UpdateSequenceNumber();

	if (myFreeLinks.empty())
	{
		ScriptLinkId id = { (unsigned int)myLinks.size() };
		myLinks.push_back({});
		return id;
	}
	ScriptLinkId id = myFreeLinks.back();
	myFreeLinks.pop_back();

	myLinks[id.id] = {};
	return id;
}
Tga::ScriptPinId Tga::Script::CreatePin()
{
	UpdateSequenceNumber();

	if (myFreePins.empty())
	{
		ScriptPinId id = { (unsigned int)myPins.size() };
		myPins.push_back({});
		return id;
	}
	ScriptPinId id = myFreePins.back();
	myFreePins.pop_back();

	myPins[id.id] = {};
	return id;
}

Tga::ScriptNodeId Tga::Script::CreateNode(ScriptNodeTypeId aTypeId, std::unique_ptr<ScriptNodeBase>&& aNewNode, Tga::Vector2f aPos)
{
	UpdateSequenceNumber();

	ScriptNodeId id = CreateNode(aTypeId);
	NodeInternalData& nodeData = myNodes[id.id];
	nodeData.node = std::move(aNewNode);
	nodeData.pos = aPos;
	return id;
}
Tga::ScriptLinkId Tga::Script::CreateLink(const ScriptLink& aLinkData)
{
	UpdateSequenceNumber();

	ScriptLinkId id = CreateLink();
	SetLink(id, aLinkData);
	return id;
}
Tga::ScriptPinId Tga::Script::CreatePin(const ScriptPin& aPinData)
{
	UpdateSequenceNumber();

	ScriptPinId id = CreatePin();
	SetPin(id, aPinData);
	return id;
}

void Tga::Script::CreateNodeWithReusedId(ScriptNodeId aId, ScriptNodeTypeId aTypeId, std::unique_ptr<ScriptNodeBase>&& aNewNode, Tga::Vector2f aPos)
{
	UpdateSequenceNumber();

	while (aId.id >= myNodes.size())
	{
		// reserve removed nodes up to size;
		myNodes.push_back({ true, 0, ScriptStringId {ScriptStringId::InvalidId} });
	}
	assert(myNodes[aId.id].isRemoved);

	for (int i = 0; i < myFreeNodes.size(); i++)
	{
		if (myFreeNodes[i] == aId)
		{
			myFreeNodes.erase(myFreeNodes.begin() + i);
			break;
		}
	}

	NodeInternalData& nodeData = myNodes[aId.id];
	nodeData = {};
	nodeData.typeId = aTypeId;
	nodeData.node = std::move(aNewNode);
	nodeData.pos = aPos;
}
void Tga::Script::CreateLinkWithReusedId(ScriptLinkId aId, const ScriptLink& aNewScriptLinkData)
{
	UpdateSequenceNumber();

	if (aId.id >= myLinks.size())
	{
		// reserve removed nodes up to size;
		myLinks.resize(aId.id + 1, { true });
	}
	assert(myLinks[aId.id].isRemoved);

	for (int i = 0; i < myFreeLinks.size(); i++)
	{
		if (myFreeLinks[i] == aId)
		{
			myFreeLinks.erase(myFreeLinks.begin() + i);
			break;
		}
	}

	myLinks[aId.id] = {};
	SetLink(aId, aNewScriptLinkData);
}
void Tga::Script::CreatePinWithReusedId(ScriptPinId aId, const ScriptPin& aNewScriptPinData)
{
	UpdateSequenceNumber();

	if (aId.id >= myPins.size())
	{
		// reserve removed nodes up to size;
		myPins.resize(aId.id + 1, { true });
	}
	assert(myPins[aId.id].isRemoved);

	for (int i = 0; i < myFreePins.size(); i++)
	{
		if (myFreePins[i] == aId)
		{
			myFreePins.erase(myFreePins.begin() + i);
			break;
		}
	}

	myPins[aId.id] = {};
	SetPin(aId, aNewScriptPinData);
}


std::unique_ptr<ScriptNodeBase> Script::RemoveNode(ScriptNodeId aId)
{
	UpdateSequenceNumber();

	assert(aId.id < myNodes.size());
	if (myNodes[aId.id].isRemoved)
		return nullptr;

	std::unique_ptr<ScriptNodeBase> node = std::move(myNodes[aId.id].node);
	SetName(aId, {});
	myNodes[aId.id] = {};
	myNodes[aId.id].isRemoved = true;
	myFreeNodes.push_back(aId);

	return node;
}
void Script::RemoveLink(ScriptLinkId aId)
{
	UpdateSequenceNumber();

	assert(aId.id < myLinks.size());
	if (myLinks[aId.id].isRemoved)
		return;

	SetLink(aId, {});
	myLinks[aId.id].isRemoved = true;
	myFreeLinks.push_back(aId);
}
void Script::RemovePin(ScriptPinId aId)
{
	UpdateSequenceNumber();

	assert(aId.id < myPins.size());
	if (myPins[aId.id].isRemoved)
		return;

	SetPin(aId, {});
	myPins[aId.id].isRemoved = true;
	myFreePins.push_back(aId);
}

bool Script::Exists(ScriptNodeId aId)
{
	UpdateSequenceNumber();

	if (aId.id >= myNodes.size())
		return false;

	return !myNodes[aId.id].isRemoved;
}
bool Script::Exists(ScriptLinkId aId)
{
	UpdateSequenceNumber();

	if (aId.id >= myLinks.size())
		return false;

	return !myLinks[aId.id].isRemoved;
}
bool Script::Exists(ScriptPinId aId)
{
	UpdateSequenceNumber();

	if (aId.id >= myPins.size())
		return false;

	return !myPins[aId.id].isRemoved;
}


ScriptNodeTypeId Script::GetType(ScriptNodeId aId) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	return myNodes[aId.id].typeId;
}
const ScriptNodeBase& Script::GetNode(ScriptNodeId aId) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	return *myNodes[aId.id].node;
}
ScriptNodeBase& Script::GetNode(ScriptNodeId aId)
{
	//UpdateSequenceNumber();

	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	return *myNodes[aId.id].node;
}

ScriptStringId Script::GetName(ScriptNodeId aId) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	return myNodes[aId.id].instanceName;
}
Tga::Vector2f Script::GetPosition(ScriptNodeId aId) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	return myNodes[aId.id].pos;
}

const ScriptPinId* Script::GetInputPins(ScriptNodeId aId, size_t& aOutCount) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	aOutCount = myNodes[aId.id].inputPins.size();
	return myNodes[aId.id].inputPins.data();
}
const ScriptPinId* Script::GetOutputPins(ScriptNodeId aId, size_t& aOutCount) const
{
	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	aOutCount = myNodes[aId.id].outputPins.size();
	return myNodes[aId.id].outputPins.data();
}

const ScriptLink& Script::GetLink(ScriptLinkId aId) const
{
	assert(aId.id < myLinks.size());
	assert(myLinks[aId.id].isRemoved == false);

	return myLinks[aId.id].link;
}
const ScriptPin& Script::GetPin(ScriptPinId aId) const
{
	assert(aId.id < myPins.size());
	assert(myPins[aId.id].isRemoved == false);

	return myPins[aId.id].pin;
}

const ScriptLinkId* Script::GetConnectedLinks(ScriptPinId aId, size_t& aOutCount) const
{
	assert(aId.id < myPins.size());
	assert(myPins[aId.id].isRemoved == false);

	aOutCount = myPins[aId.id].connectedLinks.size();
	return myPins[aId.id].connectedLinks.data();
}

void Script::SetNode(ScriptNodeId aId, std::unique_ptr<ScriptNodeBase>&& aNewNodeData)
{
	UpdateSequenceNumber();

	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);
	myNodes[aId.id].node = std::move(aNewNodeData);
}
void Script::SetName(ScriptNodeId aId, ScriptStringId aName)
{
	UpdateSequenceNumber();

	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	ScriptStringId oldName = myNodes[aId.id].instanceName;
	if (oldName.id != ScriptStringId::InvalidId)
	{
		auto pair = myInstanceToId.equal_range(oldName);
		auto it = pair.first;
		for (; it != pair.second; ++it)
		{
			if (it->second.id == aId.id)
			{
				myInstanceToId.erase(it);
				break;
			}
		}
	}
	if (aName.id != ScriptStringId::InvalidId)
	{
		myInstanceToId.insert(std::make_pair(aName, aId));
	}

	myNodes[aId.id].instanceName = aName;

}
void Script::SetPosition(ScriptNodeId aId, Tga::Vector2f aPos)
{
	UpdateSequenceNumber();

	assert(aId.id < myNodes.size());
	assert(myNodes[aId.id].isRemoved == false);

	myNodes[aId.id].pos = aPos;
}

void Script::SetLink(ScriptLinkId aId, const ScriptLink& aNewScriptLinkData)
{
	UpdateSequenceNumber();

	assert(aId.id < myLinks.size());
	assert(myLinks[aId.id].isRemoved == false);

	ScriptPinId oldSourcePin = myLinks[aId.id].link.sourcePinId;
	ScriptPinId oldTargetPin = myLinks[aId.id].link.targetPinId;

	ScriptPinId newSourcePin = aNewScriptLinkData.sourcePinId;
	ScriptPinId newTargetPin = aNewScriptLinkData.targetPinId;

	if (oldSourcePin.id != newSourcePin.id)
	{
		if (oldSourcePin.id < myPins.size() && myPins[oldSourcePin.id].isRemoved == false)
		{
			PinInternalData& pinData = myPins[oldSourcePin.id];
			for (int i = 0; i < pinData.connectedLinks.size(); i++)
			{
				if (pinData.connectedLinks[i] == aId)
				{
					pinData.connectedLinks.erase(pinData.connectedLinks.begin() + i);
					break;
				}
			}
		}

		if (newSourcePin.id < myPins.size() && myPins[newSourcePin.id].isRemoved == false)
			myPins[newSourcePin.id].connectedLinks.push_back(aId);
	}

	if (oldTargetPin.id != newTargetPin.id)
	{
		if (oldTargetPin.id < myPins.size() && myPins[oldTargetPin.id].isRemoved == false)
		{
			PinInternalData& pinData = myPins[oldTargetPin.id];
			for (int i = 0; i < pinData.connectedLinks.size(); i++)
			{
				if (pinData.connectedLinks[i] == aId)
				{
					pinData.connectedLinks.erase(pinData.connectedLinks.begin() + i);
					break;
				}
			}
		}

		if (newTargetPin.id < myPins.size() && myPins[newTargetPin.id].isRemoved == false)
			myPins[newTargetPin.id].connectedLinks.push_back(aId);
	}

	myLinks[aId.id].link = aNewScriptLinkData;
}
void Script::SetPin(ScriptPinId aId, const ScriptPin& aNewScriptPinData)
{
	UpdateSequenceNumber();

	assert(aId.id < myPins.size());
	assert(myPins[aId.id].isRemoved == false);

	ScriptPinRole oldNodeRole = myPins[aId.id].pin.role;
	ScriptPinRole newNodeRole = aNewScriptPinData.role;
	ScriptNodeId oldNodeId = myPins[aId.id].pin.node;
	ScriptNodeId newNodeId = aNewScriptPinData.node;
	int oldSortingNumber = myPins[aId.id].pin.sortingNumber;
	int newSortingNumber = aNewScriptPinData.sortingNumber;

	if (oldNodeId != newNodeId || oldNodeRole != newNodeRole || oldSortingNumber != newSortingNumber)
	{
		if (oldNodeId.id < myNodes.size() && myNodes[oldNodeId.id].isRemoved == false)
		{
			std::vector<ScriptPinId>& pinsToEdit = oldNodeRole == ScriptPinRole::Input ? myNodes[oldNodeId.id].inputPins : myNodes[oldNodeId.id].outputPins;
			for (int i = 0; i < pinsToEdit.size(); i++)
			{
				if (pinsToEdit[i] == aId)
				{
					pinsToEdit.erase(pinsToEdit.begin() + i);
					break;
				}
			}
		}

		if (newNodeId.id < myNodes.size() && myNodes[newNodeId.id].isRemoved == false)
		{
			std::vector<ScriptPinId>& pinsToEdit = newNodeRole == ScriptPinRole::Input ? myNodes[newNodeId.id].inputPins : myNodes[newNodeId.id].outputPins;
			pinsToEdit.push_back(aId);

			std::sort(begin(pinsToEdit), end(pinsToEdit), [&](ScriptPinId a, ScriptPinId b)
				{
					return myPins[a.id].pin.sortingNumber < myPins[b.id].pin.sortingNumber;
				});
		}
	}

	myPins[aId.id].pin = aNewScriptPinData;
}

// Essentially loops through for the x of Nodes to get the closest one that isn't removed
Tga::ScriptNodeId Tga::Script::GetFirstNodeId() const
{
	for (unsigned int i = 0; i < myNodes.size(); i++)
	{
		if (!myNodes[i].isRemoved)
			return { i };
	}

	return { Tga::ScriptNodeId::InvalidId };
}
Tga::ScriptNodeId Tga::Script::GetNextNodeId(ScriptNodeId aPreviousId) const
{
	for (unsigned int i = (unsigned int)aPreviousId.id + 1; i < myNodes.size(); i++)
	{
		if (!myNodes[i].isRemoved)
			return { i };
	}

	return { ScriptNodeId::InvalidId };
}
Tga::ScriptNodeId Tga::Script::GetLastNodeId() const
{
	for (unsigned int i = (unsigned int)myNodes.size(); i > 0; i--)
	{
		if (!myNodes[i - 1].isRemoved)
			return { i - 1 };
	}

	return { ScriptNodeId::InvalidId };
}
// Essentially loops through for the x of Links to get the closest one that isn't removed
Tga::ScriptLinkId Tga::Script::GetFirstLinkId() const
{
	for (unsigned int i = 0; i < myLinks.size(); i++)
	{
		if (!myLinks[i].isRemoved)
			return { i };
	}

	return { ScriptLinkId::InvalidId };
}
Tga::ScriptLinkId Tga::Script::GetNextLinkId(ScriptLinkId aPreviousId) const
{
	for (unsigned int i = (unsigned int)aPreviousId.id + 1; i < myLinks.size(); i++)
	{
		if (!myLinks[i].isRemoved)
			return { i };
	}

	return { ScriptLinkId::InvalidId };
}
Tga::ScriptLinkId Tga::Script::GetLastLinkId() const
{
	for (unsigned int i = (unsigned int)myLinks.size(); i > 0; i--)
	{
		if (!myLinks[i - 1].isRemoved)
			return { i - 1 };
	}

	return { ScriptLinkId::InvalidId };
}
// Essentially loops through for the x of Pins to get the closest one that isn't removed
Tga::ScriptPinId Tga::Script::GetFirstPinId() const
{
	for (unsigned int i = 0; i < myPins.size(); i++)
	{
		if (!myPins[i].isRemoved)
			return { i };
	}

	return { ScriptPinId::InvalidId };
}
Tga::ScriptPinId Tga::Script::GetNextPinId(ScriptPinId aPreviousId) const
{
	for (unsigned int i = (unsigned int)aPreviousId.id + 1; i < myPins.size(); i++)
	{
		if (!myPins[i].isRemoved)
			return { i };
	}

	return { ScriptPinId::InvalidId };
}
Tga::ScriptPinId Tga::Script::GetLastPinId() const
{
	for (unsigned int i = 0; i < myPins.size(); i++)
	{
		if (!myPins[i].isRemoved)
			return { i };
	}

	return { ScriptPinId::InvalidId };
}

ScriptNodeId Script::FindNodeByInstanceName(ScriptStringId aStringId) const
{
	auto pair = myInstanceToId.equal_range(aStringId);
	if (pair.first == pair.second)
		return { ScriptNodeId::InvalidId };

	auto it = pair.first;
	it++;
	if (it == pair.second)
		return pair.first->second;

	return { ScriptNodeId::InvalidId };
}