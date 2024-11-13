#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

/* 
	To save a custom class it has to have a default constructor aswell as a 
	from_json + to_json function

	Example:

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
		void to_json(json& j, const Vector3& aVector) {
			j = json{
				{"x", aVector.x},
				{"y", aVector.y},
				{"z", aVector.z}
			};
		}

		// Define how to convert JSON to a data type of choice
		void from_json(const json& j, Vector3& aVector) {
			j.at("x").get_to(aVector.x);
			j.at("y").get_to(aVector.y);
			j.at("z").get_to(aVector.z);
}
*/

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace Tools
{
	class SaveManager
	{
	public:
		SaveManager(std::string aDirectoryPath) : myDirectoryPath(aDirectoryPath)
		{
			EnsureDirectoryExists();
		};
		~SaveManager()
		{
			if (myData)
			{
				delete myData;
				myData = nullptr;
			}
		}

		template<typename T>
		void SaveData(const std::string& aKey, const T& aValue)
		{
			(*myData)[aKey] = aValue;
		}

		void LoadOrCreate(std::string aFileName)
		{
			if (myData == nullptr)
			{
				myData = new json();
			}
			myFileName = aFileName;

			LoadJsonFromFile(ConstructFilePath(myFileName), *myData);
		}
		void EndSave()
		{
			SaveJsonToFile(ConstructFilePath(myFileName), *myData);
			delete myData;
			myData = nullptr;
		}

		template<typename T>
		T GetData(std::string aKey)
		{
			if (myData && myData->contains(aKey))
			{
				auto& value = (*myData)[aKey];

				if (!value.is_null())
				{
					return (*myData)[aKey].get<T>();
				}
			}
			return T();
		}

	private:
		bool SaveJsonToFile(std::string aFileName, const json& aData)
		{
			std::ofstream outFile(aFileName);

			if (outFile.is_open())
			{
				outFile << aData.dump(4);

				outFile.close();
				return true;
			}
			// Assert?
			return false;
		}

		bool LoadJsonFromFile(std::string aFileName, json& aData)
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

		std::string ConstructFilePath(const std::string& aFileName) const
		{
			return myDirectoryPath + "/" + aFileName;
		}

		void EnsureDirectoryExists()
		{
			if (!fs::exists(myDirectoryPath))
			{
				fs::create_directories(myDirectoryPath);
			}
		}

	private:
		std::string myDirectoryPath = " ";
		std::string myFileName = "none";
		json* myData = nullptr;
	};


}