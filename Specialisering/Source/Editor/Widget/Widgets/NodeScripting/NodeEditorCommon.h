#pragma once
#include <Scripting/ScriptCommon.h>
struct ImNodesEditorContext;

namespace Tga
{
	constexpr uint8_t ScriptLinkColors[(size_t)ScriptLinkDataType::COUNT][3] =
	{
		{245, 0, 0},		// Unknown
		{185, 185, 185},	// Flow
		{126, 0,0},			// Bool
		{130, 206, 151},	// Int
		{37, 93, 140},		// Float	
		{206, 43, 206},		// String
		{206, 0, 206},		// Float2
		{206, 43, 100},		// Float4
	};
	constexpr uint8_t ScriptLinkHoverColors[(size_t)ScriptLinkDataType::COUNT][3] =
	{
		{255, 50, 50},		// Unknown
		{255, 255, 255},	// Flow
		{196, 50, 50},		// Bool
		{83, 255, 221},		// Int
		{37, 120, 200},		// Float	
		{255, 113, 255},	// String
		{255, 0, 255},		// Float2
		{236, 63, 130},		// Float4

	};
	constexpr uint8_t ScriptLinkSelectedColors[(size_t)ScriptLinkDataType::COUNT][3] =
	{
		{255, 50, 50},		// Unknown
		{255, 255, 255},	// Flow
		{196, 50, 50},		// Bool
		{83, 255, 221},		// Int
		{37, 150, 230},		// Float	
		{255, 113, 255},	// String
		{255, 0, 255},		// Float2
		{246, 73, 140},		// Float4

	};
}