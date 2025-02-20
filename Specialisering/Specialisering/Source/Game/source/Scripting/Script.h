#pragma once
#include "ScriptCommon.h"

#include <tge/math/vector2.h>
#include <string>
#include <vector>
#include <unordered_map>
namespace Tga
{
	class Script
	{
	public:
		struct NodeInternalData
		{
			bool isRemoved;
			ScriptNodeTypeId typeId;
			ScriptStringId instanceName;
			Tga::Vector2f pos;
			std::unique_ptr<ScriptNodeBase> node;
			std::vector<ScriptPinId> inputPins;
			std::vector<ScriptPinId> outputPins;
		};
		struct LinkInternalData
		{
			bool isRemoved;
			ScriptLink link;
		};
		struct PinInternalData
		{
			bool isRemoved;
			ScriptPin pin;
			std::vector<ScriptLinkId> connectedLinks;
		};

		bool myIsShaderScript = false;
		std::string myScriptName = "x";

		int mySequenceNumber = 0;
		std::vector<NodeInternalData> myNodes;
		std::vector<ScriptNodeId> myFreeNodes;

		std::vector<LinkInternalData> myLinks;
		std::vector<ScriptLinkId> myFreeLinks;

		std::vector<PinInternalData> myPins;
		std::vector<ScriptPinId> myFreePins;

		std::unordered_multimap<ScriptStringId, ScriptNodeId> myInstanceToId;

		void UpdateSequenceNumber() { mySequenceNumber++; }
	public:
		Script();
		~Script();

		void Clear();

		void LoadFromJson(const ScriptJson& aData);
		void WriteToJson(ScriptJson& aData);

		ScriptNodeId CreateNode(ScriptNodeTypeId aTypeId);
		ScriptLinkId CreateLink();
		ScriptPinId CreatePin();

		ScriptNodeId CreateNode(ScriptNodeTypeId aTypeId, std::unique_ptr<ScriptNodeBase>&& aNewNode, Tga::Vector2f aPos);
		ScriptLinkId CreateLink(const ScriptLink& aLinkData);
		ScriptPinId CreatePin(const ScriptPin& aPinData);

		/// <summary>
		/// Creates a node and reuses an existing ID. The ID may not be in use when this is called. Useful for implementing undo/redo and serialization
		/// </summary>
		void CreateNodeWithReusedId(ScriptNodeId aId, ScriptNodeTypeId aTypeId, std::unique_ptr<ScriptNodeBase>&& aNewNode, Tga::Vector2f aPos);
		/// <summary>
		/// Creates a link and reuses an existing ID.The ID may not be in use when this is called. Useful for implementing undo/redo and serialization
		/// </summary>
		void CreateLinkWithReusedId(ScriptLinkId aId, const ScriptLink& aNewScriptLinkData);
		/// <summary>
		/// Creates a Pin and reuses an existing ID. The ID may not be in use when this is called. Useful for implementing undo/redo and serialization
		/// </summary>
		void CreatePinWithReusedId(ScriptPinId aId, const ScriptPin& aNewScriptPinData);

		std::unique_ptr<ScriptNodeBase> RemoveNode(ScriptNodeId id);
		void RemoveLink(ScriptLinkId id);
		void RemovePin(ScriptPinId id);

		bool Exists(ScriptNodeId id);
		bool Exists(ScriptLinkId id);
		bool Exists(ScriptPinId id);

		ScriptNodeTypeId GetType(ScriptNodeId aId) const;
		const ScriptNodeBase& GetNode(ScriptNodeId aId) const;
		ScriptNodeBase& GetNode(ScriptNodeId aId);
		ScriptStringId GetName(ScriptNodeId aId) const;
		Tga::Vector2f GetPosition(ScriptNodeId aId) const;

		const ScriptPinId* GetInputPins(ScriptNodeId node, size_t& outCount) const;
		const ScriptPinId* GetOutputPins(ScriptNodeId node, size_t& outCount) const;

		const ScriptLink& GetLink(ScriptLinkId aId) const;
		const ScriptPin& GetPin(ScriptPinId aId) const;

		const ScriptLinkId* GetConnectedLinks(ScriptPinId pin, size_t& outCount) const;

		void SetNode(ScriptNodeId aId, std::unique_ptr<ScriptNodeBase>&& newNodeData);
		void SetName(ScriptNodeId aId, ScriptStringId name);
		void SetPosition(ScriptNodeId aId, Tga::Vector2f pos);

		void SetLink(ScriptLinkId aId, const ScriptLink& newScriptLinkData);
		void SetPin(ScriptPinId aId, const ScriptPin& newScriptPinData);


		ScriptNodeId GetFirstNodeId() const;
		ScriptNodeId GetNextNodeId(ScriptNodeId aPreviousId) const;
		ScriptNodeId GetLastNodeId() const;

		ScriptLinkId GetFirstLinkId() const;
		ScriptLinkId GetNextLinkId(ScriptLinkId aPreviousId) const;
		ScriptLinkId GetLastLinkId() const;

		ScriptPinId GetFirstPinId() const;
		ScriptPinId GetNextPinId(ScriptPinId aPreviousId) const;
		ScriptPinId GetLastPinId() const;

		/// <summary>
		/// Finds a node by instance name. Returns an invalid id if there are multiple nodes with the same name
		/// </summary>
		/// <param name="stringId"></param>
		/// <returns></returns>
		ScriptNodeId FindNodeByInstanceName(ScriptStringId aStringId) const;

		/// <summary>
		/// Used to track changes to the script. Each version of a script has a unique sequence number, except for during the execution of commands and similar constructs that alter the sequence number. 
		/// </summary>
		/// <returns></returns>
		int GetSequenceNumber() const { return mySequenceNumber; };

		/// <summary>
		/// Should only be used to implement functionality such as Commands and Undo, that combines multiple changes into one or restores older versions of the script
		/// </summary>
		/// <param name="sequenceNumber"></param>
		void SetSequenceNumber(int aSequenceNumber) { mySequenceNumber = aSequenceNumber; }


		bool IsShaderScript() { return myIsShaderScript; }
		const bool IsShaderScript() const{ return myIsShaderScript; }
		void SetIsShaderScript(bool aState) { myIsShaderScript = aState; }

		void SetScriptName(std::string aName) { myScriptName = aName; }
		std::string GetScriptName() { return myScriptName; }
		const std::string GetScriptName() const { return myScriptName; }
	};
}
