#include "MainSingleton.h"
#include <Shader/ShaderManager.h>

#include <Scripting/Nodes/DebugNodes.h>
#include <Scripting/Nodes/CommonNodes.h>
#include <Scripting/Nodes/ShaderOutputNodes.h>
#include <Scripting/Nodes/BufferNodes.h>
#include <Scripting/Nodes/MathNodes.h>

void Tga::MainSingleton::Init()
{
	Tga::RegisterCommonNodes();
	Tga::RegisterMathNodes();
	Tga::RegisterShaderOutNodes();
	Tga::RegisterBufferNodes();
	Tga::RegisterDebugNodes();

	myShaderManager = std::make_shared<ShaderManager>();
	myShaderManager->Init();

}
