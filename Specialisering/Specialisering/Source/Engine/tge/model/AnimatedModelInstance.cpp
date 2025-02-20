#include "stdafx.h"
#include "AnimatedModelInstance.h"
#include <tge/model/ModelFactory.h>
#include <tge/animation/Animation.h>
#include <tge/animation/AnimationPlayer.h>
#include <tge/animation/Pose.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/shaders/ModelShader.h>

using namespace Tga;

AnimatedModelInstance::AnimatedModelInstance()
{

}

AnimatedModelInstance::~AnimatedModelInstance()
{
	
}

void AnimatedModelInstance::Init(std::shared_ptr<Model> aModel)
{
	myModel = aModel;
}

void AnimatedModelInstance::SetTransform(const Matrix4x4f& someTransform)
{
	myTransform = someTransform;
}

void AnimatedModelInstance::Render(const ModelShader& shader) const
{
	const std::vector<Model::MeshData>& meshData = myModel->GetMeshDataList();

	for (int j = 0; j < meshData.size(); j++)
	{
		shader.Render(myTextures[j], meshData[j], myTransform, myBoneTransforms);
	}
}

void AnimatedModelInstance::Render(const ModelShader& shader, int aMeshIndex) const
{
	const std::vector<Model::MeshData>& meshData = myModel->GetMeshDataList();

	assert(aMeshIndex < meshData.size());
	if (aMeshIndex < meshData.size())
	{
		shader.Render(myTextures[aMeshIndex], meshData[aMeshIndex], myTransform, myBoneTransforms);
	}
}

void AnimatedModelInstance::SetPose(const LocalSpacePose& pose)
{
	ModelSpacePose modelSpacePose;
	myModel->GetSkeleton()->ConvertPoseToModelSpace(pose, modelSpacePose);

	SetPose(modelSpacePose);
}

void AnimatedModelInstance::SetPose(const ModelSpacePose& pose)
{
	myModel->GetSkeleton()->ApplyBindPoseInverse(pose, myBoneTransforms);
}

void AnimatedModelInstance::SetPose(const AnimationPlayer& animationInstance)
{
	SetPose(animationInstance.GetLocalSpacePose());
}

void AnimatedModelInstance::ResetPose()
{
	for (int i = 0; i < MAX_ANIMATION_BONES; i++)
	{
		myBoneTransforms[i] = Matrix4x4f::CreateIdentityMatrix();
	}
}
