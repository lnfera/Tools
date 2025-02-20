#pragma once
#include <variant>
#include <tge/math/Vector.h>
//Forward Declarations
namespace Tga
{
	class Script;
	struct ScriptJson;
	class ScriptNodeBase;
	class ScriptCreationContext;
	class ScriptExecutionContext;
	class ScriptRuntimeInstance;
	struct ScriptUpdateContext;
	class ScriptNodeRuntimeInstanceBase;
}

namespace Tga
{
	// When adding new data types make sure to add them to the lists :
	/*	-ScriptLinkDataTypeNames
	/*	-ScriptLinkData::Variant<.., Data>
	*	-ScriptLinkData::SAVE & LOAD logic in ScriptCommon.cpp file
	*/
	enum class ScriptLinkDataType
	{
		UNKNOWN = 0,
		Flow,
		Bool,
		Int,
		Float,
		String,
		Float4,
		COUNT
	};

	constexpr char* ScriptLinkDataTypeNames[(size_t)ScriptLinkDataType::COUNT] =
	{
		"Unknown",
		"Flow",
		"Bool",
		"Int",
		"Float",
		"String",
		"Float4"
	};

	enum class ScriptPinRole
	{
		Input,
		Output,
	};

	enum class ScriptNodeResult
	{
		Finished,
		KeepRunning,
	};

	struct ScriptStringId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptStringId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptStringId& other) const { return this->id != other.id; }
	};

	struct ScriptLinkData
	{
		enum VariantType
		{
			Monostate = 0,
			Bool,
			Int,
			Float,
			String,
			Float4,
		};

		// Add new data into this std::variant
		std::variant<std::monostate, bool, int, float, ScriptStringId,Vector4f> data;
		void LoadFromJson(ScriptLinkDataType aType, const ScriptJson& aJsonData);
		void WriteToJson(ScriptLinkDataType aType, ScriptJson& aJsonData) const;
	};

	struct ScriptPinId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptPinId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptPinId& other) const { return this->id != other.id; }
	};

	struct ScriptNodeId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptNodeId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptNodeId& other) const { return this->id != other.id; }
	};

	struct ScriptLinkId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptLinkId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptLinkId& other) const { return this->id != other.id; }
	};

	struct ScriptNodeTypeId
	{
		constexpr static unsigned int InvalidId = 0xFFFFFFFF;
		unsigned int id = InvalidId;

		bool operator==(const ScriptNodeTypeId& other) const { return this->id == other.id; }
		bool operator!=(const ScriptNodeTypeId& other) const { return this->id != other.id; }
	};

	struct ScriptLink
	{
		ScriptPinId sourcePinId;
		ScriptPinId targetPinId;
	};

	struct ScriptPin
	{
		ScriptPinRole role;
		int sortingNumber;
		ScriptNodeId node;
		ScriptStringId name;
		ScriptLinkDataType dataType;
		ScriptLinkData defaultValue;
		ScriptLinkData overridenValue;
	};
}

template<>
struct std::hash<Tga::ScriptStringId>
{
	std::size_t operator()(Tga::ScriptStringId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptPinId>
{
	std::size_t operator()(Tga::ScriptPinId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptNodeId>
{
	std::size_t operator()(Tga::ScriptNodeId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptLinkId>
{
	std::size_t operator()(Tga::ScriptLinkId const& id) const noexcept
	{
		return id.id;
	}
};

template<>
struct std::hash<Tga::ScriptNodeTypeId>
{
	std::size_t operator()(Tga::ScriptNodeTypeId const& id) const noexcept
	{
		return id.id;
	}
};
