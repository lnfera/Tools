#include "stdafx.h"

#include <tge/drawers/ModelDrawer.h>
#include <tge/shaders/ModelShader.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/DX11.h>
#include <tge/engine.h>
#include <tge/math/Matrix2x2.h>
#include <tge/model/AnimatedModelInstance.h>
#include <tge/model/ModelInstance.h>

using namespace Tga;

ModelDrawer::ModelDrawer()
{
}

ModelDrawer::~ModelDrawer()
{
}

bool ModelDrawer::Init()
{
	myDefaultShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myDefaultShader->Init())
	{
		return false;
	}

	myDefaultAnimatedModelShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myDefaultAnimatedModelShader->Init(L"shaders/animated_model_shader_VS.cso", L"shaders/model_shader_PS.cso"))
	{
		return false;
	}

	myPbrShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myPbrShader->Init(L"Shaders/PbrModelShaderVS.cso", L"Shaders/PbrModelShaderPS.cso"))
	{
		return false;
	}

	myPbrAnimatedModelShader = std::make_unique<ModelShader>(Engine::GetInstance());
	if (!myPbrAnimatedModelShader->Init(L"Shaders/AnimatedPbrModelShaderVS.cso", L"Shaders/PbrModelShaderPS.cso"))
	{
		return false;
	}

	return true;
}

void ModelDrawer::Draw(const AnimatedModelInstance& modelInstance)
{
	modelInstance.Render(*myDefaultAnimatedModelShader);
}

void ModelDrawer::Draw(const ModelInstance& modelInstance)
{
	modelInstance.Render(*myDefaultShader);
}

void ModelDrawer::DrawPbr(const AnimatedModelInstance& modelInstance)
{
	modelInstance.Render(*myPbrAnimatedModelShader);
}

void ModelDrawer::DrawPbr(const ModelInstance& modelInstance)
{
	modelInstance.Render(*myPbrShader);
}

void ModelDrawer::Draw(const AnimatedModelInstance& modelInstance, const ModelShader& shader)
{
	modelInstance.Render(shader);
}

void ModelDrawer::Draw(const ModelInstance& modelInstance, const ModelShader& shader)
{
	modelInstance.Render(shader);
}