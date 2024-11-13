#include <iostream>

#include <SaveTool.hpp>
#include <nlohmann/json.hpp>

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

void to_json(json& j, const Vector3& aVector) {
	j = json{
		{"x", aVector.x},
		{"y", aVector.y},
		{"z", aVector.z}
	};
}

// Define how to convert JSON to a Vector3 struct
void from_json(const json& j, Vector3& aVector) {
	j.at("x").get_to(aVector.x);
	j.at("y").get_to(aVector.y);
	j.at("z").get_to(aVector.z);
}

struct Player
{
	std::string name;
	int health;
	Vector3 position;
	std::vector<int>numbers;
};

void to_json(json& j, const Player& aPlayer) {
	j = json{
		{"name", aPlayer.name},
		{"health", aPlayer.health},
		{"position", aPlayer.position},
		{"numbers", aPlayer.numbers},
	};
}

// Define how to convert JSON to a Vector3 struct
void from_json(const json& j, Player& aPlayer) {
	j.at("name").get_to(aPlayer.name);
	j.at("health").get_to(aPlayer.health);
	j.at("position").get_to(aPlayer.position);
	j.at("numbers").get_to(aPlayer.numbers);
}

int main()
{
	Tools::SaveManager manager("json/");

	manager.LoadOrCreate("Player");
	{
		Player player = manager.GetData<Player>("Player");

		std::cout << "Name: " << player.name << '\n';
		std::cout << "Health: " << player.health << '\n';

		manager.SaveData("Player", player);
	}
	manager.EndSave();

	manager.LoadOrCreate("World");
	{
		std::vector<Vector3> positions = manager.GetData<std::vector<Vector3>>("Positions");

		if (positions.empty())
		{
			for (int i = 0; i < 10; i++)
			{
				positions.push_back(Vector3());
			}
		}
		manager.SaveData<std::vector<Vector3>>("Positions", positions);
	}
	manager.EndSave();
}
