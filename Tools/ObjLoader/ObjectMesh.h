#pragma once
#include <vector>
namespace Tga
{
	struct v3
	{
		float x = 0, y = 0, z, = 0;
	};

	struct Face {
		std::vector<int> vertexIndices; // Indices to vertex data
		std::vector<int> texCoordIndices; // Indices to texture coordinate data
		std::vector<int> normalIndices; // Indices to normal data
	};

	class ObjMesh
	{
	public:
		ObjMesh();
		~ObjMesh();

		bool LoadFromFile(const std::string& aFileName, int aSizeMod = 10);

		void SetPosition(Vector3f aPos) { myPosition = aPos; }

		std::string GetPath() { return myPath; }

		const std::vector<Face>& GetFaces() { return myFaces; }
		const std::vector<v3>& GetVerticies() { return myVertices; }
		const std::vector<v3>& GetNormals() { return myNormals; }
		const std::vector<int>& GetIndicies() { return myIndices; }

	private:
		std::string myPath;
		Vector3f myPosition;

		std::vector<Face> myFaces;

		std::vector<v3> myVertices;
		std::vector<int> myIndices;
		std::vector<v3> myNormals;
	};
}