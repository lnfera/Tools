#pragma once 
#include "AbstractCommand.h"
#include <vector>
#include <tge/math/Matrix4x4.h>
namespace Tga
{
	class GameObject;
	class TransformCommand : public AbstractCommand
	{
	public:
		TransformCommand() = default;

		void Execute() override;
		void Undo() override;
		~TransformCommand() override;

		void BeginCommand(std::vector<GameObject*> aGroupOfObjects);
		void EndCommand();
	private:
		
		std::vector<GameObject*> myObjectsToMove;
		std::vector<Matrix4x4f> myBeginObjects;
		std::vector<Matrix4x4f> myEndTransforms;

		//Matrix4x4f myFromTransform;
		//Matrix4x4f myToTransform;
	};
}