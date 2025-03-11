#include "ScriptNodeTypeRegistry.h"

#include "ScriptStringRegistry.h"

#include <cassert>
#include <sstream>

using namespace Tga;

std::unordered_map<std::string_view, ScriptNodeTypeId> ScriptNodeTypeRegistry::myStringToTypeId;
std::vector<ScriptNodeTypeRegistry::TypeInfo> ScriptNodeTypeRegistry::myTypeInfos;
ScriptNodeTypeRegistry::CategoryInfo ScriptNodeTypeRegistry::myRootCategory = {};


ScriptNodeTypeId Tga::ScriptNodeTypeRegistry::GetTypeId(std::string_view aTypeName)
{
	auto it = myStringToTypeId.find(aTypeName);
	if (it != myStringToTypeId.end())
		return it->second;

	return { 0xFFFFFFFF };
}
//
//ScriptNodeTypeId Tga::ScriptNodeTypeRegistry::GetTypeId(int aTypeHash)
//{
//	size_t typeHash = aTypeHash;
//	for (const auto& entry : myStringToTypeId)
//	{
//	}
//
//}

std::unique_ptr<ScriptNodeBase> Tga::ScriptNodeTypeRegistry::CreateNode(ScriptNodeTypeId aTypeId)
{
	assert("Invalid type id" && aTypeId.id < myTypeInfos.size());

	return (*myTypeInfos[aTypeId.id].createNodeFunctionPtr)();
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTypeShortName(ScriptNodeTypeId aTypeId)
{
	assert("Invalid type id" && aTypeId.id < myTypeInfos.size());

	return myTypeInfos[aTypeId.id].shortName;
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTypeFullName(ScriptNodeTypeId aTypeId)
{
	assert("Invalid type id" && aTypeId.id < myTypeInfos.size());

	return myTypeInfos[aTypeId.id].fullName;
}

std::string_view Tga::ScriptNodeTypeRegistry::GetNodeTooltip(ScriptNodeTypeId aTypeId)
{
	assert("Invalid type id" && aTypeId.id < myTypeInfos.size());

	return myTypeInfos[aTypeId.id].toolTip;
}

ScriptNodeTypeRegistry::TypeInfo& ScriptNodeTypeRegistry::RegisterTypeInternal(const char* aFullName, const char* aToolTip)
{
	ScriptNodeTypeId typeId = { (unsigned int)myTypeInfos.size() };

	TypeInfo& typeInfo = myTypeInfos.emplace_back();

	typeInfo.fullName = aFullName;

	const char* shortName = strrchr(aFullName, '/');
	if (shortName == nullptr)
		shortName = aFullName;
	else
		shortName += 1; // skip the '/'
	typeInfo.shortName = shortName;

	std::istringstream f(aFullName);
	std::string s;

	CategoryInfo* category = &myRootCategory;
	while (std::getline(f, s, '/'))
	{
		ScriptStringId stringId = ScriptStringRegistry::RegisterOrGetString(s.c_str());
		typeInfo.path.push_back(stringId);
	}

	for (int pathIndex = 0; pathIndex + 1 < typeInfo.path.size(); pathIndex++)
	{
		ScriptStringId stringId = typeInfo.path[pathIndex];
		int childCategoryIndex = -1;
		for (int i = 0; i < category->childCategories.size(); i++)
		{
			if (category->childCategories[i].name == stringId)
			{
				childCategoryIndex = i;
			}
		}

		if (childCategoryIndex == -1)
		{
			childCategoryIndex = (int)category->childCategories.size();
			CategoryInfo& childCategory = category->childCategories.emplace_back();
			childCategory.name = stringId;
		}

		category = &category->childCategories[childCategoryIndex];
	}

	category->nodeTypes.push_back(typeId);

	typeInfo.toolTip = aToolTip;

	assert("Node type name (without category) already exists. Duplicates are not allowed" && myStringToTypeId.find(typeInfo.shortName) == myStringToTypeId.end());
	assert("Node type name (including cagories) already exists. Duplicates are not allowed" && myStringToTypeId.find(typeInfo.fullName) == myStringToTypeId.end());

	myStringToTypeId[typeInfo.shortName] = typeId;
	myStringToTypeId[typeInfo.fullName] = typeId;

	return typeInfo;
}
