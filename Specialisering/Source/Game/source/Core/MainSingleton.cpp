#include "MainSingleton.h"
#include <Shader/ShaderManager.h>

#include <Scripting/Nodes/DebugNodes.h>
#include <Scripting/Nodes/CommonNodes.h>
#include <Scripting/Nodes/ShaderOutputNodes.h>
#include <Scripting/Nodes/BufferNodes.h>
#include <Scripting/Nodes/MathNodes.h>
#include <Scripting/Nodes/InputNodes.h>
#include "MainDrawer.h"
void Tga::MainSingleton::Init()
{
	Tga::RegisterInputNodes();
	Tga::RegisterCommonNodes();
	Tga::RegisterMathNodes();
	Tga::RegisterShaderOutNodes();
	Tga::RegisterBufferNodes();
	Tga::RegisterDebugNodes();

	myShaderManager = std::make_shared<ShaderManager>();
	myShaderManager->Init();

	myMainDrawer = std::make_shared<MainDrawer>();
	myMainDrawer->Init();

}
