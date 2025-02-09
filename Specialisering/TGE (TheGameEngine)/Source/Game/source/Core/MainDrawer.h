#pragma once
#include <string>
#include <vector>
#include <unordered_map>
namespace Tga
{
	class RenderTarget;
	class ModelInstance;
}

namespace Tga
{
	struct RenderData
	{
		ModelInstance* modelInstance;
	};


	class MainDrawer
	{
	public:
		MainDrawer();
		~MainDrawer();

		bool Init();

		void RenderToTarget(RenderTarget* aTarget);
	private:

	private:
		std::unordered_map<std::string, RenderData> myRenderData;
	};
}


/*	
*	Clear Buffers
*	Shadow Camera?
*		
*	Defered
*	-	Render Objects sorted by Model, Shader etc. to target
*	-	Assemble
*	-	Draw Points Lights Etc.
*	-	Apply Post Processing
* 
*	Draw UI
*	
*/