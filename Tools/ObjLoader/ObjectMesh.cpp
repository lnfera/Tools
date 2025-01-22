#include "ObjectMesh.h"
#include <filesystem>
#include <fstream>
#include <sstream>

#include <tge/drawers/LineDrawer.h>
#include <tge/primitives/LinePrimitive.h>
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
Tga::ObjMesh::ObjMesh()
{
}

Tga::ObjMesh::~ObjMesh()
{
}

bool Tga::ObjMesh::LoadFromFile(const std::string& aFileName, int aSizeMod)
{
    myPath = aFileName;

    std::ifstream file(aFileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << aFileName << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v") {
            float x,y,z;
            lineStream >> x >>y >> z;
            myVertices.push_back({x * aSizeMod,y * aSizeMod,z * aSizeMod});
        }
        else if (prefix == "vn") {
            float nX,nY,nZ;
            lineStream >> nX >> nY >> nZ;
            myNormals.push_back({nX ,nY,nZ });
        }
        else if (prefix == "f") {
            std::vector<int> vertexIndices, texCoordIndices, normalIndices;
            std::string vertexData;

            while (lineStream >> vertexData) {
                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::istringstream vertexStream(vertexData);
                int vertexIndex, texCoordIndex, normalIndex;

                vertexStream >> vertexIndex;
                vertexIndices.push_back(vertexIndex - 1); // OBJ is 1-indexed
                if (vertexStream.peek() == ' ') vertexStream.ignore();
                if (vertexStream >> texCoordIndex) texCoordIndices.push_back(texCoordIndex - 1);
                if (vertexStream.peek() == ' ') vertexStream.ignore();
                if (vertexStream >> normalIndex) normalIndices.push_back(normalIndex - 1);
            }

            // Triangulate the face
            for (size_t i = 1; i < vertexIndices.size() - 1; ++i) {
                Face triangle;
                triangle.vertexIndices = { vertexIndices[0], vertexIndices[i], vertexIndices[i + 1] };

                if (!texCoordIndices.empty()) {
                    triangle.texCoordIndices = { texCoordIndices[0], texCoordIndices[i], texCoordIndices[i + 1] };
                }

                if (!normalIndices.empty()) {
                    triangle.normalIndices = { normalIndices[0], normalIndices[i], normalIndices[i + 1] };
                }

                myFaces.push_back(triangle);
            }
        }
    }

    file.close();
    return true;
}

void Tga::ObjMesh::Render()
{
    auto& drawer = Tga::Engine::GetInstance()->GetGraphicsEngine().GetLineDrawer();

    Tga::LinePrimitive line;
    line.color = myColor;
    for(int i = 0; i < myFaces.size(); i++)
    {
        line.fromPosition = myVertices[myFaces[i].vertexIndices[0]] + myPosition;
        line.toPosition = myVertices[myFaces[i].vertexIndices[1]] + myPosition;
        drawer.Draw(line);

        line.fromPosition = myVertices[myFaces[i].vertexIndices[2]] + myPosition;
        line.toPosition = myVertices[myFaces[i].vertexIndices[1]] + myPosition;
        drawer.Draw(line);

        line.fromPosition = myVertices[myFaces[i].vertexIndices[2]] + myPosition;
        line.toPosition = myVertices[myFaces[i].vertexIndices[0]] + myPosition;
        drawer.Draw(line);
    }
}
