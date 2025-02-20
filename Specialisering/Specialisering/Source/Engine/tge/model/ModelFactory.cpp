#include "stdafx.h"
#include "ModelFactory.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include <tge/animation/animationPlayer.h>
#include <tge/graphics/DX11.h>
#include <tge/util/StringCast.h>
#include <tge/model/Model.h>
#include <tge/model/ModelInstance.h>
#include <tge/graphics/Vertex.h>
#include <tge/math/matrix4x4.h>
#include <tge/texture/texture.h>
#include <tge/texture/TextureManager.h>

#include <TGAFBXImporter/source/Importer.h>
#include <DDSTextureLoader/DDSTextureLoader11.h>

using namespace Tga;
ModelFactory* ModelFactory::myInstance = nullptr;

#define TEXTURE_SET_0 0
#define TEXTURE_SET_1 1
#define TEXTURE_SET_2 2
#define TEXTURE_SET_3 3

#define VERTEX_COLOR_SET_0 0
#define VERTEX_COLOR_SET_1 1
#define VERTEX_COLOR_SET_2 2
#define VERTEX_COLOR_SET_3 3

#define NUM_BONES_PER_VERTEX 4

using namespace Tga;

//Matrix4x4f ConvertToEngineMatrix33(const aiMatrix3x3& AssimpMatrix)
//{
//    Matrix4x4f mat;
//    for (int i = 0; i < 3; i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            mat(1 + i, 1 + j) = AssimpMatrix[i][j];
//        }
//    }
//
//    return mat;
//}
//
//// constructor from Assimp matrix
//Matrix4x4f ConvertToEngineMatrix4x4f(const aiMatrix4x4& AssimpMatrix)
//{
//    Matrix4x4f mat;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            mat(1 + i, 1 + j) = AssimpMatrix[i][j];
//        }
//    }
//    return mat;
//}
//
//inline bool operator <(const aiString& aLhs, const aiString& aRhs)
//{
//    return strcmp(aLhs.data, aRhs.data) < 0;
//}

struct VertexBoneData
{
	unsigned int IDs[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weights, 0, sizeof(Weights));
	}

	void AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++)
		{
			if (Weights[i] == 0.0)
			{
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		//assert(0);
	}
};

bool ModelFactory::InitUnitCube()
{
	// First we make a cube.

	Model::MeshData meshData = {};
	// Watch the winding! DX defaults to Clockwise.
	// Assume the winding as if you're viewing the face head on.
	// +Y up, +X right, +Z Forward
	meshData.Vertices = 
	{
		// Front
		{
			50.0f, -50.0f, 50.0f,
			0, 0, 1,
			1, 0, 0,
			0, -1, 0,
			1, 1, 1, 1,
			0, 1
		},
		{
			50.0f, 50.0f, 50.0f,
			0, 0, 1,
			1, 0, 0,
			0, -1, 0,
			1, 1, 1, 1,
			0, 0
		},
		{
			-50.0f, 50.0f, 50.0f,
			0, 0, 1,
			1, 0, 0,
			0, -1, 0,
			1, 1, 1, 1,
			1, 0
		},
		{
			-50.0f, -50.0f, 50.0f,
			0, 0, 1,
			1, 0, 0,
			0, -1, 0,
			1, 1, 1, 1,
			1, 1
		},

		// Left
		{
			-50.0f, -50.0f, 50.0f,
			-1, 0, 0,
			0, 0, 1,
			0, -1, 0,
			1, 0, 0, 1,
			0, 1
		},
		{
			-50.0f, 50.0f, 50.0f,
			-1, 0, 0,
			0, 0, 1,
			0, -1, 0,
			1, 0, 0, 1,
			0, 0
		},
		{
			-50.0f, 50.0f, -50.0f,
			-1, 0, 0,
			0, 0, 1,
			0, -1, 0,
			1, 0, 0, 1,
			1, 0
		},
		{
			-50.0f, -50.0f, -50.0f,
			-1, 0, 0,
			0, 0, 1,
			0, -1, 0,
			1, 0, 0, 1,
			1, 1
		},

		// Back
		{
			-50.0f, -50.0f, -50.0f,
			0, 0, -1,
			1, 0, 0,
			0, -1, 0,
			0, 1, 0, 1,
			0, 1
		},
		{
			-50.0f, 50.0f, -50.0f,
			0, 0, -1,
			1, 0, 0,
			0, -1, 0,
			0, 1, 0, 1,
			0, 0
		},
		{
			50.0f, 50.0f, -50.0f,
			0, 0, -1,
			1, 0, 0,
			0, -1, 0,
			0, 1, 0, 1,
			1, 0
		},
		{
			50.0f, -50.0f, -50.0f,
			0, 0, -1,
			1, 0, 0,
			0, -1, 0,
			0, 1, 0, 1,
			1, 1
		},

		// Right
		{
			50.0f, -50.0f, -50.0f,
			1, 0, 0,
			0, 0, -1,
			0, -1, 0,
			0, 0, 1, 1,
			0, 1
		},
		{
			50.0f, 50.0f, -50.0f,
			1, 0, 0,
			0, 0, -1,
			0, -1, 0,
			0, 0, 1, 1,
			0, 0
		},
		{
			50.0f, 50.0f, 50.0f,
			1, 0, 0,
			0, 0, -1,
			0, -1, 0,
			0, 0, 1, 1
			, 1, 0
		},
		{
			50.0f, -50.0f, 50.0f,
			1, 0, 0,
			0, 0, -1,
			0, -1, 0,
			0, 0, 1, 1,
			1, 1
		},

		// Top
		{
			50.0f, 50.0f, 50.0f,
			0, 1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 1, 0, 1,
			0, 1
		},
		{
			50.0f, 50.0f, -50.0f,
			0, 1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 1, 0, 1,
			0, 0
		},
		{
			-50.0f, 50.0f, -50.0f,
			0, 1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 1, 0, 1,
			1, 0
		},
		{
			-50.0f, 50.0f, 50.0f,
			0, 1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 1, 0, 1,
			1, 1
		},

		// Bottom
		{
			-50.0f, -50.0f, 50.0f,
			0, -1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 0, 1, 1,
			0, 1
		},
		{
			-50.0f, -50.0f, -50.0f,
			0, -1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 0, 1, 1,
			0, 0
		},
		{
			50.0f, -50.0f, -50.0f,
			0, -1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 0, 1, 1,
			1, 0
		},
		{
			50.0f, -50.0f, 50.0f,
			0, -1, 0,
			1, 0, 0,
			0, 0, 1,
			1, 0, 1, 1
			, 1, 1
		},
	};

	meshData.Indices =
	{
		0, 1, 2,        /* |/ */
		0, 2, 3,        /* /| */
		4, 5, 6,        /* |/ */
		4, 6, 7,        /* /| */
		8, 9, 10,       /* |/ */
		8, 10, 11,      /* /| */
		12, 13, 14,     /* |/ */
		12, 14, 15,     /* /| */
		16, 17, 18,     /* |/ */
		16, 18, 19,     /* /| */
		20, 21, 22,     /* |/ */
		20, 22, 23      /* /| */
	};

	//const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
	//const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
	//const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));

	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Vertices.size()) * static_cast<UINT>(sizeof(Vertex));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &meshData.Vertices[0];

	ID3D11Buffer* vertexBuffer;
	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Indices.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	indexSubresourceData.pSysMem = &meshData.Indices[0];

	ID3D11Buffer* indexBuffer;
	result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::shared_ptr<Model> model = std::make_shared<Model>();

	meshData.NumberOfVertices = static_cast<UINT>(meshData.Vertices.size());
	meshData.NumberOfIndices = static_cast<UINT>(meshData.Indices.size());
	meshData.Stride = sizeof(Vertex);
	meshData.Offset = 0;
	meshData.VertexBuffer = vertexBuffer;
	meshData.IndexBuffer = indexBuffer;
	meshData.Bounds = CalculateBoxSphereBounds(meshData.Vertices);
	model->Init(meshData, L"Cube");
	myLoadedModels.insert(std::pair<std::wstring, std::shared_ptr<Model>>(L"Cube", model));

	return true;
}

bool ModelFactory::InitUnitPlane()
{
	Model::MeshData meshData = {};

	meshData.Vertices.push_back({
		-50.0f, 0.0f, 50.0f,
		0, 1, 0,
		1, 0, 0,
		0, 0, 1,
		1, 1, 1, 1,
		0, 0
		});

	meshData.Vertices.push_back({
		50.0f, 0.0f, 50.0f,
		0, 1, 0,
		1, 0, 0,
		0, 0, 1,
		1, 1, 1, 1,
		1, 0
		});

	meshData.Vertices.push_back({
		50.0f, 0.0f, -50.0f,
		0, 1, 0,
		1, 0, 0,
		0, 0, 1,
		1, 1, 1, 1,
		1, 1
		});

	meshData.Vertices.push_back({
		-50.0f, 0.0f, -50.0f,
		0, 1, 0,
		1, 0, 0,
		0, 0, 1,
		1, 1, 1, 1,
		0, 1
		});


	meshData.Indices = { 0, 1, 2, 0, 2, 3 };

	//const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
	//const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
	//const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));

	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Vertices.size()) * static_cast<UINT>(sizeof(Vertex));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &meshData.Vertices[0];

	ID3D11Buffer* vertexBuffer;
	result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Indices.size()) * static_cast<UINT>(sizeof(unsigned int));
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubresourceData{};
	indexSubresourceData.pSysMem = &meshData.Indices[0];

	ID3D11Buffer* indexBuffer;
	result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	std::shared_ptr<Model> model = std::make_shared<Model>();

	meshData.NumberOfVertices = static_cast<UINT>(meshData.Vertices.size());
	meshData.NumberOfIndices = static_cast<UINT>(meshData.Indices.size());
	meshData.Stride = sizeof(Vertex);
	meshData.Offset = 0;
	meshData.VertexBuffer = vertexBuffer;
	meshData.IndexBuffer = indexBuffer;
	meshData.Bounds = CalculateBoxSphereBounds(meshData.Vertices);
	model->Init(meshData, L"Plane");
	myLoadedModels.insert(std::pair<std::wstring, std::shared_ptr<Model>>(L"Plane", model));

	return true;
}

bool ModelFactory::InitPrimitives()
{
	if (!InitUnitCube())
		return false;

	if (!InitUnitPlane())
		return false;

	return true;
}

ModelFactory::ModelFactory()
{
	TGA::FBX::Importer::InitImporter();
	InitPrimitives();
	myInstance = this;
}

ModelFactory::~ModelFactory()
{
	myInstance = nullptr;
	TGA::FBX::Importer::UninitImporter();
}

static TextureResource *AssignAlbedoTexture(const std::wstring& baseFileName, const std::wstring &materialFileName) {
	Engine& engine = *Engine::GetInstance();
	std::wstring fnAlbedo = materialFileName + L"_C.dds";
	TextureResource* albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());

	if (albedoTexture == nullptr)
	{
		fnAlbedo = materialFileName + L"_D.dds";
		albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
	}

	if (albedoTexture == nullptr)
	{
		fnAlbedo = baseFileName + L"_C.dds";
		albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
	}

	if (albedoTexture == nullptr)
	{
		fnAlbedo = baseFileName + L"_D.dds";
		albedoTexture = engine.GetTextureManager().TryGetTexture(fnAlbedo.c_str());
	}

	if (albedoTexture == nullptr)
	{
		albedoTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_c.dds");
	}
	return albedoTexture;
}

static TextureResource *AssignNormalTexture(const std::wstring& baseFileName, const std::wstring &materialFileName) {
	Engine& engine = *Engine::GetInstance();
	std::wstring fnNormal = materialFileName + L"_N.dds";
	TextureResource *normalTexture = engine.GetTextureManager().TryGetTexture(fnNormal.c_str(), TextureSrgbMode::ForceNoSrgbFormat);

	if (normalTexture == nullptr)
	{
		fnNormal = baseFileName + L"_N.dds";
		normalTexture = engine.GetTextureManager().TryGetTexture(fnNormal.c_str(), TextureSrgbMode::ForceNoSrgbFormat);
	}

	if (normalTexture == nullptr)
		normalTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_n.dds", TextureSrgbMode::ForceNoSrgbFormat);

	return normalTexture;
}

static TextureResource *AssignMaterialTexture(const std::wstring& baseFileName, const std::wstring &materialFileName) {
	Engine& engine = *Engine::GetInstance();
	std::wstring fnMaterial = materialFileName + L"_M.dds";
	TextureResource *materialTexture = engine.GetTextureManager().TryGetTexture(fnMaterial.c_str(), TextureSrgbMode::ForceNoSrgbFormat);

	if (materialTexture == nullptr)
	{
		fnMaterial = baseFileName + L"_M.dds";
		materialTexture = engine.GetTextureManager().TryGetTexture(fnMaterial.c_str(), TextureSrgbMode::ForceNoSrgbFormat);
	}

	if (materialTexture == nullptr)
		materialTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_m.dds", TextureSrgbMode::ForceNoSrgbFormat);

	return materialTexture;
}

static TextureResource *AssignFxTexture(const std::wstring& baseFileName, const std::wstring &materialFileName) {
	Engine& engine = *Engine::GetInstance();
	std::wstring fnFx = materialFileName + L"_FX.dds";
	TextureResource *fxTexture = engine.GetTextureManager().TryGetTexture(fnFx.c_str(), TextureSrgbMode::ForceNoSrgbFormat);

	if (fxTexture == nullptr)
	{
		fnFx = baseFileName + L"_FX.dds";
		fxTexture = engine.GetTextureManager().TryGetTexture(fnFx.c_str(), TextureSrgbMode::ForceNoSrgbFormat);
	}

	if (fxTexture == nullptr)
		fxTexture = engine.GetTextureManager().GetTexture(L"Textures/T_Default_fx.dds", TextureSrgbMode::ForceNoSrgbFormat);
	return fxTexture;
}

template<typename ModelInstanceType>
void AssignMaterials(const std::wstring& someFilePath, Model* Mdl, ModelInstanceType& MI)
{
	std::wstring baseFileName = someFilePath.substr(0, someFilePath.find_last_of(L'.'));
	std::wstring path = someFilePath.substr(0, someFilePath.find_last_of(L"/\\") + 1);

	for (int i = 0; i < Mdl->GetMeshCount(); i++)
	{
		std::wstring filename = baseFileName.substr(baseFileName.find_last_of(L"/\\") + 1, baseFileName.length());
		std::wstring materialFileName = path + string_cast<std::wstring>(Mdl->GetMaterialName(i));

		TextureResource* albedoTexture = AssignAlbedoTexture(baseFileName, materialFileName);
		MI.SetTexture(i, 0, albedoTexture);

		TextureResource* normalTexture = AssignNormalTexture(baseFileName, materialFileName);
		MI.SetTexture(i, 1, normalTexture);

		TextureResource* materialTexture = AssignMaterialTexture(baseFileName, materialFileName);
		MI.SetTexture(i, 2, materialTexture);

		TextureResource* fxTexture = AssignFxTexture(baseFileName, materialFileName);
		MI.SetTexture(i, 3, fxTexture);
	}
}

template<typename ModelInstanceType>
static void AssignMaterials(Model* aModel, ModelInstanceType& aModelInstance, const nlohmann::json& json) {
	std::string fbxPath = json["Fbx"];
	std::string baseFileName = fbxPath.substr(0, fbxPath.find_last_of('.'));
	std::string path = fbxPath.substr(0, fbxPath.find_last_of("/\\") + 1);

	for (int i = 0; i < aModel->GetMeshCount(); i++)
	{
		std::string filename = baseFileName.substr(baseFileName.find_last_of("/\\") + 1, baseFileName.length());
		std::string materialFileName = path + aModel->GetMaterialName(i);

		Tga::Engine& engine = *Tga::Engine::GetInstance();
		TextureManager& textureManager = engine.GetTextureManager();

		const nlohmann::json& texturesJson = json.contains("Textures") ? json["Textures"] : nlohmann::json({});

		TextureResource* albedoTexture = nullptr;
		TextureResource* normalTexture = nullptr;
		TextureResource* materialTexture = nullptr;
		TextureResource* fxTexture = nullptr;
	
		if (texturesJson.contains("Color")) {
			std::string string = texturesJson["Color"];
			std::wstring texturePath = Settings::ResolveAssetPathW(string_cast<std::wstring>(string));
			albedoTexture = texturePath.length() > 0 ? textureManager.TryGetTexture(texturePath.c_str()) : nullptr;
		}
		if (texturesJson.contains("Normal")) {
			std::string string = texturesJson["Normal"];
			std::wstring texturePath = Settings::ResolveAssetPathW(string_cast<std::wstring>(string));
			normalTexture = texturePath.length() > 0 ? textureManager.TryGetTexture(texturePath.c_str()) : nullptr;
		}
		if (texturesJson.contains("Material")) {
			std::string string = texturesJson["Material"];
			std::wstring texturePath = Settings::ResolveAssetPathW(string_cast<std::wstring>(string));
			materialTexture = texturePath.length() > 0 ? textureManager.TryGetTexture(texturePath.c_str()) : nullptr;
		}
		if (texturesJson.contains("Fx")) {
			std::string string = texturesJson["Fx"];
			std::wstring texturePath = Settings::ResolveAssetPathW(string_cast<std::wstring>(string));
			fxTexture = texturePath.length() > 0 ? textureManager.TryGetTexture(texturePath.c_str()) : nullptr;
		}

		if (albedoTexture == nullptr) {
			albedoTexture = AssignAlbedoTexture(string_cast<std::wstring>(baseFileName), string_cast<std::wstring>(materialFileName));
		}
		if (normalTexture == nullptr) {
			normalTexture = AssignNormalTexture(string_cast<std::wstring>(baseFileName), string_cast<std::wstring>(materialFileName));
		}
		if (materialTexture == nullptr) {
			materialTexture = AssignMaterialTexture(string_cast<std::wstring>(baseFileName), string_cast<std::wstring>(materialFileName));
		}
		if (fxTexture == nullptr) {
			fxTexture = AssignFxTexture(string_cast<std::wstring>(baseFileName), string_cast<std::wstring>(materialFileName));
		}

		aModelInstance.SetTexture(i, 0, albedoTexture);
		aModelInstance.SetTexture(i, 1, normalTexture);
		aModelInstance.SetTexture(i, 2, materialTexture);
		aModelInstance.SetTexture(i, 3, fxTexture);
	}
}

ModelInstance ModelFactory::GetModelInstance(const std::wstring& someFilePath)
{
	// This needs to be moved to separate memory structures at some point.
	ModelInstance MI;

	std::shared_ptr<Model> Mdl = GetModel(someFilePath);
	if (!Mdl)
		return MI;

	MI.Init(Mdl);

	std::wstring path = someFilePath;
	if (someFilePath.find(L".") != std::string::npos)
	{
		path = Tga::Settings::ResolveAssetPathW(someFilePath);
	}
	AssignMaterials(path, Mdl.get(), MI);

	return MI;
}

ModelInstance ModelFactory::GetTgmModelInstance(const std::wstring& aTgmName) {
	ModelInstance MI;

	std::wstring tgmPath = Settings::ResolveAssetPathW(aTgmName);
	std::ifstream file(tgmPath);
	if (file.good())
	{
		const nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		{ // load fbx specified in tgm file
			std::string fbxPath = json["Fbx"];
			std::shared_ptr<Model> Mdl = GetModel(string_cast<std::wstring>(fbxPath));
			if (!Mdl) {
				return MI;
			}

			MI.Init(Mdl);
			AssignMaterials(Mdl.get(), MI, json);
		}
	}

	return MI;
}

AnimatedModelInstance ModelFactory::GetAnimatedTgmModelInstance(const std::wstring& aTgmName)
{
	AnimatedModelInstance MI;
	std::wstring tgmPath = Tga::Settings::ResolveAssetPathW(aTgmName);
	std::ifstream file(tgmPath);
	if (file.good())
	{
		const nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		{ // load fbx specified in tgm file
			std::string fbxPath = json["Fbx"];
			std::shared_ptr<Model> Mdl = GetModel(string_cast<std::wstring>(fbxPath));
			if (!Mdl || !Mdl->GetSkeleton()->GetRoot())
				return MI;

			MI.Init(Mdl);
			AssignMaterials(Mdl.get(), MI, json);
		}

	}

	return MI;
}

AnimatedModelInstance ModelFactory::GetAnimatedModelInstance(const std::wstring& someFilePath)
{
	// This needs to be moved to separate memory structures at some point.
	AnimatedModelInstance MI;
	std::wstring resolved_path = Tga::Settings::ResolveAssetPathW(someFilePath);

	std::shared_ptr<Model> Mdl = GetModel(resolved_path);
	if (!Mdl || !Mdl->GetSkeleton()->GetRoot())
		return MI;

	MI.Init(Mdl);

	AssignMaterials(resolved_path, Mdl.get(), MI);

	return MI;
}

ModelInstance ModelFactory::GetUnitCube()
{
	return GetModelInstance(L"Cube");
}

ModelInstance ModelFactory::GetUnitPlane()
{
	return GetModelInstance(L"Plane");
}

bool ModelFactory::ModelHasMesh(const std::wstring&)
{
	return true;
	// New FBX does not have this function, It checked if the file was an fbx
	//const std::string ansiFileName = string_cast<std::string>(someFilePath);
	//return TGA::FBX::Importer::IsValidModelFile(ansiFileName);
}

std::shared_ptr<Model> ModelFactory::LoadModelW(const std::wstring& someFilePath)
{
	// The FBX SDK doesn't like widechar :(.
	//const std::string ansiFileName = string_cast<std::string>(someFilePath);
	std::wstring resolved_path = Tga::Settings::ResolveAssetPathW(someFilePath);
	const std::string_view path = Tga::Settings::ResolveAssetPath(string_cast<std::string>(resolved_path));

	TGA::FBX::Mesh tgaModel;
	if (TGA::FBX::Importer::LoadMeshA(path.data(), tgaModel))
	{
		Skeleton mdlSkeleton;

		if (tgaModel.Skeleton.GetRoot())
		{
			mdlSkeleton.Joints.resize(tgaModel.Skeleton.Bones.size());
			mdlSkeleton.JointNameToIndex.reserve(mdlSkeleton.Joints.size());
			mdlSkeleton.JointName.resize(mdlSkeleton.Joints.size());
			for (size_t j = 0; j < tgaModel.Skeleton.Bones.size(); j++)
			{
				Skeleton::Joint& mdlJoint = mdlSkeleton.Joints[j];
				TGA::FBX::Skeleton::Bone& tgaJoint = tgaModel.Skeleton.Bones[j];

				Matrix4x4f bindPoseInverseTranspose;
				memcpy(&bindPoseInverseTranspose, &tgaJoint.BindPoseInverse, sizeof(float) * 16);

				mdlJoint.BindPoseInverse = Matrix4x4f::Transpose(bindPoseInverseTranspose);
				mdlJoint.Name = tgaJoint.Name;
				mdlJoint.Parent = tgaJoint.ParentIdx;
				mdlJoint.Children = tgaJoint.Children;

				mdlSkeleton.JointNameToIndex.insert({ mdlJoint.Name, j });
				mdlSkeleton.JointName[j] = mdlJoint.Name;
			}
			assert(MAX_ANIMATION_BONES >= mdlSkeleton.Joints.size() && "More joints in animation than defined in EngingeDefines.h");
		}

		std::vector<Model::MeshData> mdlMeshData;
		mdlMeshData.resize(tgaModel.Elements.size());

		// Convert model to our own format.
		for (size_t i = 0; i < tgaModel.Elements.size(); i++)
		{
			// The imported element data.
			TGA::FBX::Mesh::Element& element = tgaModel.Elements[i];

			// And where we'll put it in our structures.
			Model::MeshData& meshData = mdlMeshData[i];
			meshData.Vertices.resize(element.Vertices.size());

			// Convert vertices to your own format
			//std::vector<Vertex> mdlVertices;
			//mdlVertices.resize(element.Vertices.size());

			for (size_t v = 0; v < element.Vertices.size(); v++)
			{
				// The most important part, the position!
				meshData.Vertices[v].Position = {
					element.Vertices[v].Position[0],
					element.Vertices[v].Position[1],
					element.Vertices[v].Position[2],
					element.Vertices[v].Position[3]
				};

				// All four vertex color channels I have.
				for (int vCol = 0; vCol < 4; vCol++)
				{
					meshData.Vertices[v].VertexColors[vCol] = {
						element.Vertices[v].VertexColors[vCol][0],
						element.Vertices[v].VertexColors[vCol][1],
						element.Vertices[v].VertexColors[vCol][2],
						element.Vertices[v].VertexColors[vCol][3]
					};

					//FIX ME. Sets vertex colors to 1 if no data given
					// Daniel s�ger att FBX inportern l�ser vertex collors r�tt, och om den l�ser f�rgerna r�tt s� f�r den det v�rdet. 
					// Vilket leder till svart i vertex colors -> modeller blev transparenta.
					if (element.Vertices[v].VertexColors[vCol][0] + element.Vertices[v].VertexColors[vCol][1] + element.Vertices[v].VertexColors[vCol][2] + element.Vertices[v].VertexColors[vCol][3] == 0)
					{
						meshData.Vertices[v].VertexColors[vCol] = { 1,1,1,1 };
					}

				}

				meshData.Vertices[v].Normal = Vector3f(element.Vertices[v].Normal[0], element.Vertices[v].Normal[1], element.Vertices[v].Normal[2]);
				meshData.Vertices[v].Binormal = Vector3f(element.Vertices[v].BiNormal[0], element.Vertices[v].BiNormal[1], element.Vertices[v].BiNormal[2]);
				meshData.Vertices[v].Tangent = Vector3f(element.Vertices[v].Tangent[0], element.Vertices[v].Tangent[1], element.Vertices[v].Tangent[2]);

				for (unsigned int UVch = 0; UVch < 4; UVch++)
				{
					meshData.Vertices[v].UVs[UVch] = {
						 element.Vertices[v].UVs[UVch][0],
						 element.Vertices[v].UVs[UVch][1]
					};
				}

				meshData.Vertices[v].Bones = {
					static_cast<float>(element.Vertices[v].BoneIDs[0]),
					static_cast<float>(element.Vertices[v].BoneIDs[1]),
					static_cast<float>(element.Vertices[v].BoneIDs[2]),
					static_cast<float>(element.Vertices[v].BoneIDs[3])
				};

				meshData.Vertices[v].Weights = {
					element.Vertices[v].BoneWeights[0],
					element.Vertices[v].BoneWeights[1],
					element.Vertices[v].BoneWeights[2],
					element.Vertices[v].BoneWeights[3]
				};
			}

			//std::vector<unsigned int>& mdlIndices = element.Indices;
			meshData.Indices.resize(element.Indices.size());
			memcpy(meshData.Indices.data(), element.Indices.data(), sizeof(unsigned int) * element.Indices.size());
			//meshData.Indices = element.Indices;

			HRESULT result;

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Vertices.size()) * static_cast<UINT>(sizeof(Vertex));
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
			vertexSubresourceData.pSysMem = &meshData.Vertices[0];

			ID3D11Buffer* vertexBuffer;
			result = DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
			if (FAILED(result))
			{
				return nullptr;
			}

			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.ByteWidth = static_cast<UINT>(meshData.Indices.size()) * static_cast<UINT>(sizeof(float)); // TODO: What :P Sizeof should be uint.
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubresourceData{};
			indexSubresourceData.pSysMem = &meshData.Indices[0];

			ID3D11Buffer* indexBuffer;
			result = DX11::Device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
			if (FAILED(result))
			{
				return nullptr;
			}

			meshData.NumberOfVertices = static_cast<UINT>(meshData.Vertices.size());
			meshData.NumberOfIndices = static_cast<UINT>(meshData.Indices.size());
			meshData.Stride = sizeof(Vertex);
			meshData.Offset = 0;
			meshData.VertexBuffer = vertexBuffer;
			meshData.IndexBuffer = indexBuffer;
			meshData.Name = element.MeshName;
			if (tgaModel.Materials.size() > element.MaterialIndex)
			{
				meshData.MaterialName = tgaModel.Materials[element.MaterialIndex].MaterialName;
			}
			else
			{
				meshData.MaterialName = "";
			}
			meshData.Bounds = CalculateBoxSphereBounds(meshData.Vertices);
		}

		std::shared_ptr<Model> model = std::make_shared<Model>();

		model->Init(mdlMeshData, resolved_path);
		if (mdlSkeleton.Joints.size() > 0)
		{
			model->mySkeleton = std::move(mdlSkeleton);
		}

		myLoadedModels.insert(std::pair<std::wstring, std::shared_ptr<Model>>(someFilePath, model));

		return model;
	}

	return nullptr;
}

Tga::BoxSphereBounds Tga::ModelFactory::CalculateBoxSphereBounds(std::vector<Tga::Vertex> somePositions)
{
	Vector3f minExtents = Vector3f::Zero;
	Vector3f maxExtents = Vector3f::Zero;

	for (unsigned int v = 0; v < somePositions.size(); v++)
	{
		if (somePositions[v].Position.x > maxExtents.X)
			maxExtents.X = somePositions[v].Position.x;
		if (somePositions[v].Position.y > maxExtents.Y)
			maxExtents.Y = somePositions[v].Position.y;
		if (somePositions[v].Position.z > maxExtents.Z)
			maxExtents.Z = somePositions[v].Position.z;

		if (somePositions[v].Position.x < minExtents.X)
			minExtents.X = somePositions[v].Position.x;
		if (somePositions[v].Position.y < minExtents.Y)
			minExtents.Y = somePositions[v].Position.y;
		if (somePositions[v].Position.z < minExtents.Z)
			minExtents.Z = somePositions[v].Position.z;
	}

	const Vector3f extentsCenter = 0.5f * (minExtents + maxExtents);
	const Vector3f boxExtents = 0.5f * (maxExtents - minExtents);
	const float myBoxSphereRadius = FMath::Max(boxExtents.X, FMath::Max(boxExtents.Y, boxExtents.Z));
	return { myBoxSphereRadius, boxExtents, extentsCenter };
}

std::shared_ptr<Animation> ModelFactory::GetAnimation(const std::wstring& someFilePath, const std::shared_ptr<Model>& aModel)
{
	// The FBX SDK doesn't like widechar :(.
	const std::string ansiFileName = string_cast<std::string>(someFilePath);
	TGA::FBX::Animation fbxAnimation;

	auto it = myLoadedAnimations.find(AnimationIdentifer{ someFilePath , aModel });
	if (it != myLoadedAnimations.end())
		return it->second;

	if (TGA::FBX::Importer::LoadAnimationA(ansiFileName, fbxAnimation))
	{
		std::shared_ptr<Animation> animation = std::make_shared<Animation>();
		animation->Name = string_cast<std::wstring>(fbxAnimation.Name);
		animation->Length = fbxAnimation.Length;
		animation->FramesPerSecond = fbxAnimation.FramesPerSecond;
		animation->Frames.resize(fbxAnimation.Frames.size());
		animation->Duration = static_cast<float>(fbxAnimation.Duration);

		const Tga::Skeleton& skeleton = *aModel->GetSkeleton();
		
		for (size_t f = 0; f < animation->Frames.size(); f++)
		{
			for (const auto& [boneName, boneTransform] : fbxAnimation.Frames[f].LocalTransforms)
			{
				Matrix4x4f localMatrix;
				memcpy_s(&localMatrix, sizeof(Matrix4x4f), boneTransform.Data, sizeof(float) * 16);

				auto jointIt = skeleton.JointNameToIndex.find(boneName);

				if (jointIt != skeleton.JointNameToIndex.end())
					animation->Frames[f].JointTransforms[jointIt->second] = ScaleRotationTranslationf::CreateFromMatrix(localMatrix);
			}
		}

		myLoadedAnimations[AnimationIdentifer{ someFilePath , aModel }] = animation;

		return animation;
	}

	return nullptr;
}

AnimationPlayer ModelFactory::GetAnimationPlayer(const std::wstring& someFilePath, const std::shared_ptr<Model>& aModel)
{
	AnimationPlayer instance;
	std::shared_ptr<Animation> animation = GetAnimation(Tga::Settings::ResolveAssetPathW(someFilePath), aModel);
	if (animation)
		instance.Init(animation, aModel);

	return instance;
}

std::shared_ptr<Model> ModelFactory::GetModel(const std::wstring& someFilePath)
{
	auto it = myLoadedModels.find(someFilePath);
	if (it != myLoadedModels.end())
		return it->second;

	return LoadModelW(someFilePath);
}