#pragma once
#include <fstream>
namespace Tools
{
	template <typename T>
	void SaveToBinary(T& aValue, FILE* aFile)
	{
		aFile; aValue;
		fwrite(&aValue, sizeof(T), 1, aFile);

	};
	template <typename T>
	T LoadFromBinary(FILE* aFile)
	{
		T value;
		fread(&value, sizeof(T), 1, aFile);
		return value;
	};

#pragma region StringLoad
	// [String Save & Load]
	void SaveToBinary(std::string& aValue, FILE* aFile)
	{
		uint32_t len = (uint32_t)aValue.size();
		fwrite(&len, sizeof(len), 1, aFile);

		fwrite(aValue.c_str(), sizeof(char), len + 1, aFile);
	};
	template<>
	std::string LoadFromBinary(FILE* aFile)
	{
		std::string output;

		uint32_t len = 0;
		fread(&len, sizeof(uint32_t), 1, aFile);

		char* name = (char*)malloc((len + 1) * sizeof(char));
		fread(name, sizeof(char), len + 1, aFile);
		output = name;
		free(name);
		name = nullptr;

		return output;
	};

	void SaveToBinary(std::wstring aString, FILE* aFile)
	{
		uint32_t len = (uint32_t)aString.size();
		fwrite(&len, sizeof(len), 1, aFile);

		fwrite(aString.c_str(), sizeof(wchar_t), len + 1, aFile);
	}
	template<>
	std::wstring LoadFromBinary(FILE* aFile)
	{
		std::wstring output;

		uint32_t len = 0;
		fread(&len, sizeof(uint32_t), 1, aFile);

		if (len <= 0)
		{
			return L" ";
		}

		wchar_t* name = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
		fread(name, sizeof(wchar_t), len + 1, aFile);
		output = name;
		free(name);
		name = nullptr;

		return output;
	}
#pragma endregion

#pragma region MathSaveAndLoad

#pragma endregion

#pragma region Containers
	template <typename K, typename V>
	void SaveToBinary(std::unordered_map<K, V>& aValue, FILE* aFile)
	{
		// Number of elements
		int size = (int)aValue.size();
		fwrite(&size, sizeof(int), 1, aFile);

		for (auto& [key, value] : aValue)
		{
			SaveToBinary(key, aFile);
			SaveToBinary(value, aFile);
		}

	}
	template <typename K, typename V>
	void LoadFromBinary(FILE* aFile, std::unordered_map<K, V>& aMap)
	{
		aMap.clear();
		int size = LoadFromBinary<int>(aFile);
		for (int i = 0; i < size; i++)
		{
			K key;
			V value;

			key = LoadFromBinary<K>(aFile);
			value = LoadFromBinary<V>(aFile);

			aMap[key] = value;
		}
	}

	template <typename T>
	void SaveToBinary(std::vector<T>& aValue, FILE* aFile)
	{
		// Number of elements
		int size = (int)aValue.size();
		fwrite(&size, sizeof(int), 1, aFile);

		for (int i = 0; i < size; i++)
		{
			SaveToBinary<T>(aValue[i], aFile);
		}
	}
	template <typename T>
	void LoadFromBinary(FILE* aFile, std::vector<T>& aVector)
	{
		aVector.clear();
		int size = LoadFromBinary<int>(aFile);
		aVector.resize(size);

		for (int i = 0; i < size; i++)
		{
			aVector[i] = LoadFromBinary<T>(aFile);
		}
	}
#pragma endregion
}