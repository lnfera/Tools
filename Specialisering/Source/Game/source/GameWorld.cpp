#include "GameWorld.h"

#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/GraphicsStateStack.h>

#include <tge/io/Input.h>

#include <Scripting/Context/ScriptUpdateContext.h>
#include <Scripting/ScriptManager.h>


#include <tge/shaders/ModelShader.h>
#include "Object/I_AllComponents.h"
#include <tge/graphics/DX11.h>

#include <Shader/ShaderCompileCommon.h>
#include <Shader/DXShader.h>

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}
#include <Core/MainSingleton.h>
#include <Shader/ShaderManager.h>
void GameWorld::Init()
{
	auto& engine = *Tga::Engine::GetInstance();

	Tga::Vector2ui resolution = engine.GetRenderSize();
	Tga::Vector2f resolutionF = { (float)resolution.x , (float)resolution.y };

	myCamera.SetPerspectiveProjection(90, resolutionF, 0.1f, 10000.f);

	myScene = new Tga::Scene("empty");
	myScene->LoadFromJson("World_01");
	myScene->Init();
	myScene->Enable();

	Tga::ModelShader* shader = Tga::MainSingleton::GetInstance().GetShaderManager()->GetShader("Cosinus_Shader_PS");

	myScene->GetGameObjects()[0]->GetComponent<Tga::RenderComponent>()->myShader = shader;
	

}

void GameWorld::Update(float aDeltaTime)
{
	TGE_I()->GetGraphicsEngine().GetGraphicsStateStack().SetCamera(myCamera);


	Tga::GameUpdateContext context;
	context.deltaTime = aDeltaTime;
	context.activeObject = myScene->GetGameObjects()[0];

	myScene->Update(aDeltaTime);

	MoveCamera(aDeltaTime);
}

void GameWorld::Render()
{
	myScene->Render();
}

void GameWorld::MoveCamera(float aDeltaTime)
{
	aDeltaTime;
	Tga::Matrix4x4f& trans = myCamera.GetTransform();
	Tga::Vector3f pos = trans.GetPosition();

	//Move w. controls
	Tga::InputManager* input = &Tga::Engine::GetInstance()->GetInputManager();

	static float moveSpeed = 35.f;
	static float rotationSpeed = 0.2f;

	if (input->GetDeltaScroll() != 0)
	{
		moveSpeed += input->GetDeltaScroll() * 0.3f;
		if (moveSpeed < 0)
		{
			moveSpeed = 1.0f;
		}
	}

	if (input->IsKeyDown('W'))
	{
		pos += trans.GetForward() * moveSpeed * aDeltaTime;
	}
	if (input->IsKeyDown('S'))
	{
		pos -= trans.GetForward() * moveSpeed * aDeltaTime;
	}
	if (input->IsKeyDown('D'))
	{
		pos += trans.GetRight() * moveSpeed * aDeltaTime;
	}
	if (input->IsKeyDown('A'))
	{
		pos -= trans.GetRight() * moveSpeed * aDeltaTime;
	}

	Tga::Vector2f mDelta = input->GetMouseDelta();

	if (input->IsKeyDown(VK_RBUTTON))
	{
		if (mDelta.x != 0)
		{
			trans.Rotate({ 0,0,(rotationSpeed * mDelta.x),0});

			//trans.AddRotation({ 0,(rotationSpeed * mDelta.x),0 });
		}
		if (mDelta.y != 0)
		{

			trans.Rotate({0 ,(rotationSpeed * mDelta.y),0,0 });
			//trans.AddRotation({ (rotationSpeed * mDelta.y),0,0 });
		}
	}

	trans.SetPosition(pos);
}

