#include "GameWorld.h"
#include <tge/engine.h>
#include <tge/texture/texture.h>
#include <tge/texture/TextureManager.h>


GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
    auto& engine = *Tga::Engine::GetInstance();

    Tga::Vector2ui resolution = engine.GetRenderSize();
    Tga::Vector2f resolutionF = { (float)resolution.x , (float)resolution.y };

    myCamera.SetPerspectiveProjection(90, resolutionF, 0.1f, 10000.f);

    /*for (int i = 0; i < 10; i++)
    {
        Tga::GameObject* object = new Tga::GameObject(i);
        object->GetTransform().SetPosition({ i * 5, 0 ,i * 5 });




        myObjects.push_back(object);
    }*/
}

void GameWorld::Update(float aDeltaTime)
{
    MoveCamera(aDeltaTime);
}

void GameWorld::Render()
{

}

void GameWorld::MoveCamera(float aDeltaTime)
{
    aDeltaTime;
    Tga::Transform& trans = myCamera.GetTransform();
    Tga::Vector3f pos = trans.GetPosition();

    //Move w. controls
    //LE::InputManager* input = &Tga::Engine::GetInstance()->GetInputManager();

    static float moveSpeed = 50.f;
    static float rotationSpeed = 0.2f;

    //if (input->GetDeltaScroll() != 0)
    //{
    //    moveSpeed += input->GetDeltaScroll() * 0.3f;
    //    if (moveSpeed < 0)
    //    {
    //        moveSpeed = 1.0f;
    //    }
    //}

    //if (input->IsKeyDown('W'))
    //{
    //    pos += trans.GetMatrix().GetForward() * moveSpeed * aDeltaTime;
    //}
    //if (input->IsKeyDown('S'))
    //{
    //    pos -= trans.GetMatrix().GetForward() * moveSpeed * aDeltaTime;
    //}
    //if (input->IsKeyDown('D'))
    //{
    //    pos += trans.GetMatrix().GetRight() * moveSpeed * aDeltaTime;
    //}
    //if (input->IsKeyDown('A'))
    //{
    //    pos -= trans.GetMatrix().GetRight() * moveSpeed * aDeltaTime;
    //}

    //CU::Vector2<float> mDelta = input->GetMouseDelta();


    //if (input->IsKeyDown(VK_RBUTTON))
    //{
    //    if (mDelta.x != 0)
    //    {
    //        trans.AddRotation({ 0,(rotationSpeed * mDelta.x),0 });
    //    }
    //    if (mDelta.y != 0)
    //    {

    //        trans.AddRotation({ (rotationSpeed * mDelta.y),0,0 });
    //    }
    //}

    //myCamera.SetPosition(pos);
}

