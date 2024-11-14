#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

#define NO_DISCARD [[nodiscard]]

/*
	To save a custom class it has to have a default constructor aswell as a
	from_json + to_json function

	Example:

	namespace Example
	{
		//Create the class that we want to create
		struct Vector3
		{
				Vector3() = default;
				Vector3(float aX, float aY, float aZ)
				{
					x = aX;
					y = aY;
					z = aZ;
				}

				float x, y, z;
		};

		// Define how to convert datatype to json
		void to_json(json& j, const Vector3& aVector) 
		{
				j = json{
					{"x", aVector.x},
					{"y", aVector.y},
					{"z", aVector.z}
				};
		}

		// Define how to convert JSON to a data type of choice
		void from_json(const json& j, Vector3& aVector) 
		{
				j.at("x").get_to(aVector.x);
				j.at("y").get_to(aVector.y);
				j.at("z").get_to(aVector.z);
		}
	}
*/

/*	[Example]
	
	void SaveToolTest()
	{
		Tools::SaveManager saveManager("json/");

		saveManager.LoadOrCreate("data");

		Example::Vector3 v = saveManager.GetData<Example::Vector3>("Vector");

		//Use the data, change it or whatever

		saveManager.SaveData("Vector", v);

		saveManager.EndSave();
	}
*/

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace Tools
{
	class SaveManager
	{
	public:
		//Sets the directory path that the files will be read from and put in when EndSave() is called.
		SaveManager(std::string aDirectoryPath);
		~SaveManager();

		//Opens a document that GetData reads from and EndSave writes to.
		void LoadOrCreate(std::string aFileName);

		//Pushes the saved data to the file opened with LoadOrCreate(...).
		void EndSave();

		//Saves a variable in the temporary data file. Permanently save using EndSave when all saves have been declared.
		template<typename T>
		void SaveData(const std::string& aKey, const T& aValue);

		//Reads data from the json data file created with LoadOrCreate(...).
		template<typename T>
		NO_DISCARD T GetData(std::string aKey);

	private:
		bool SaveJsonToFile(std::string aFileName, const json& aData);

		bool LoadJsonFromFile(std::string aFileName, json& aData);

		std::string ConstructFilePath(const std::string& aFileName) const;

		void CreateDirectoryIfNoExist();

	private:
		std::string myDirectoryPath = " ";
		std::string myFileName = "ERROR_NO_NAME_SET";
		json* myData = nullptr;
	};

	//###############   [ PUBLIC FUNCTIONS ]   ###############
	SaveManager::SaveManager(std::string aDirectoryPath) : myDirectoryPath(aDirectoryPath)
	{
		CreateDirectoryIfNoExist();
	};
	SaveManager::~SaveManager()
	{
		if (myData)
		{
			delete myData;
			myData = nullptr;
		}
	}

	void SaveManager::LoadOrCreate(std::string aFileName)
	{
		if (myData == nullptr)
		{
			myData = new json();
		}
		myFileName = aFileName;

		LoadJsonFromFile(ConstructFilePath(myFileName), *myData);
	}

	void SaveManager::EndSave()
	{
		SaveJsonToFile(ConstructFilePath(myFileName), *myData);
		delete myData;
		myData = nullptr;
	}

	template<typename T>
	void SaveManager::SaveData(const std::string& aKey, const T& aValue)
	{
		(*myData)[aKey] = aValue;
	}

	template<typename T>
	T SaveManager::GetData(std::string aKey)
	{
		auto& value = (*myData)[aKey];

		if (!value.is_null())
		{
			return (*myData)[aKey].get<T>();
		}
		return T();
	}


	//###############	[ PRIVATE FUNCTIONS ]   ###############
	bool SaveManager::SaveJsonToFile(std::string aFileName, const json& aData)
	{
		std::ofstream outFile(aFileName);

		if (outFile.is_open())
		{
			outFile << aData.dump(4);

			outFile.close();
			return true;
		}
		return false;
	}

	bool SaveManager::LoadJsonFromFile(std::string aFileName, json& aData)
	{
		std::ifstream inFile(aFileName);
		if (inFile.is_open())
		{
			inFile >> aData;
			inFile.close();
			return true;
		}
		return false;
	}

	std::string SaveManager::ConstructFilePath(const std::string& aFileName) const
	{
		return myDirectoryPath + "/" + aFileName;
	}

	void SaveManager::CreateDirectoryIfNoExist()
	{
		if (!fs::exists(myDirectoryPath))
		{
			fs::create_directories(myDirectoryPath);
		}
	}
}