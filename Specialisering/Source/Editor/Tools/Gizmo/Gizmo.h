#pragma once
#include <memory>
#include <vector>

#include <tge/Math/Matrix4x4.h>
//#include <Tools/ToolsInterface.h>

#include "../Commands/TransformCommand.h"

namespace Tga
{
	class Scene;
	class Camera;
	struct Transform;
	class ModelInstance;

	class Gizmos
	{
	public:
		struct Snap { float pos = 10.f; float rot = 5.f; float scale = 0.1f; };
		Gizmos() = default;

		void DrawSettings();

		void DrawGizmos(Tga::Camera& aCamera);

	private:
		Matrix4x4f myGizmoStartTransformInverse;
		Matrix4x4f myGizmoTransform;


		TransformCommand myTransformCommand;

		uint16_t myCurrentOperation = 7;
		uint16_t myCurrentMode = 0;

		bool myIsManipulating = false;

		bool myIsSnappingActive = false;
		Snap mySnap = {0.5f, 10.0f, 0.1f};

	};
}