#include "stdafx.h"
#include "CAnimatedMeshNode.h"
#include "CShaderVariableInjection.h"
#include "ISceneManager.h"

namespace gf
{

	bool CAnimatedMeshNode::setMaterial(IMaterial* material, u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return false;

		mMaterials[subset] = material;
		return true;
	}

	IMaterial* CAnimatedMeshNode::getMaterial(u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return nullptr;

		return mMaterials[subset];
	}

	bool CAnimatedMeshNode::setMaterialName(const std::string& name, u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return false;

		IMaterial* material = mSceneManager->getVideoDriver()->getMaterialManager()->get(name);
		if (material == nullptr)
			return false;

		mMaterials[subset] = material;
		return true;
	}

	void CAnimatedMeshNode::updateSubsetBones(const std::vector<SModelSubsetBone>& bones)
	{
		for (u32 i = 0; i < bones.size(); i++)
		{
			u32 boneIndex = bones[i].BoneIndex;
			XMMATRIX toRootMatrix = XMLoadFloat4x4(&mAbsoluteBoneTransforms[boneIndex]);
			XMMATRIX offsetMatrix = XMLoadFloat4x4(&bones[i].OffsetTransform);

			XMMATRIX combined = offsetMatrix * toRootMatrix;
			combined = XMMatrixTranspose(combined);

			XMStoreFloat4x4(&mSubsetBoneTransforms[i], combined);
		}
	}

	void CAnimatedMeshNode::render()
	{
		XMFLOAT4X4 preAbsoluteTransform;

		mMesh->bind();

		IPipeline* prePipeline = nullptr; /* ��¼ǰһ����ˮ�� */
		IMaterial* preMaterial = nullptr; /* ��¼ǰһ���Ӽ��Ĳ��� */

		u32 subsetCount = mMesh->getSubsetCount();

		updateAbsoluteBoneTransforms();
		const std::vector<SAnimatedModelSubset>& subsets = mMesh->getSubsets();

		for (u32 i = 0; i < subsetCount; i++)
		{
			IMaterial* material = mMaterials[i];
			if (!material)
				continue;

			/* transform subset bones. */
			if (subsets[i].Skinned)
			{
				updateSubsetBones(subsets[i].Bones);
			}
			else
			{
				// ������Ӽ��޶���������Ҫ�ı�����任
				u32 boneId = subsets[i].BoneId;
				if (boneId >= 0)
				{
					preAbsoluteTransform = mAbsoluteTransformation;
					XMFLOAT4X4 boneTransform = mAbsoluteBoneTransforms[boneId];
					XMMATRIX absoluteMatrix = XMLoadFloat4x4(&mAbsoluteTransformation);
					XMMATRIX boneMatrix = XMLoadFloat4x4(&boneTransform);
					XMStoreFloat4x4(&mAbsoluteTransformation, boneMatrix * absoluteMatrix);
				}
			}

			for (u32 j = 0; j < material->getPipelineCount(); j++)
			{
				IPipeline* pipeline = material->getPipeline(j);
				if (!pipeline)
					continue;

				//	IPipeline* pipeline2 = subsets[0].Material->getPipeline(0);
				//pipeline = subsets[subsets.size() - 1].Material->getPipeline(0);

				// ������Ӽ��ж����������任������Ҫע���
				if (subsets[i].Skinned)
				{
					CShaderVariableInjection::injectBoneTransforms(pipeline,
						reinterpret_cast<f32*>(&mSubsetBoneTransforms[0]),
						subsets[i].Bones.size());
				}

				CShaderVariableInjection::inject(this, pipeline, i);
				pipeline->apply();
				/*

				//�����ǰ����ˮ�ߺ���һ����ͬ������˲������������������ע��
				if (pipeline == prePipeline)
				{
				//���ҽ�������Ҳ��ͬ����һ��ʱ��ע��
				if (preMaterial != material)
				{
				CShaderVariableInjection::injectMaterial(material, pipeline);
				pipeline->apply();
				}
				}
				else // ���������ˮ�߲�ͬ��ȫ����������Ҫ����
				{
				CShaderVariableInjection::inject(this, pipeline, i);
				pipeline->apply();
				prePipeline = pipeline;
				}
				*/

				mMesh->drawSubset(i);

			}

			// �������任�����˸ı䣬����Ҫ�ظ� 
			if (!subsets[i].Skinned && subsets[i].BoneId >= 0)
			{
				mAbsoluteTransformation = preAbsoluteTransform;
			}

			//preMaterial = material;
		}

	}

	void CAnimatedMeshNode::OnRegisterSceneNode(bool bRecursion)
	{
		if (mVisible)
		{
			calcSortCode();
			mSceneManager->registerNodeForRendering(this);

			if (bRecursion)
				ISceneNode::OnRegisterSceneNode();
		}
	}

	void CAnimatedMeshNode::calcSortCode()
	{
		mSortCode = ((u64)mMesh->getSortCode() << 56);

		IMaterial* material = mMaterials[0];
		if (material)
		{
			IPipeline* pipeline = material->getPipeline(0);
			if (pipeline)
			{
				mSortCode |= (pipeline->getSortCode() << 8);
			}

			ITexture* texture = material->getTexture(0);
			if (texture)
			{
				mSortCode |= texture->getSortCode();
			}
		}
	}

	bool CAnimatedMeshNode::setAnimation(u32 id)
	{
		if (id >= mMesh->getAnimationCount())
			return false;

		mCurrAnimationId = id;
		return true;
	}

	bool CAnimatedMeshNode::setAnimation(const std::string& name)
	{
		s32 id = mMesh->getAnimationId(name);
		if (id == -1)
			return false;

		mCurrAnimationId = id;
		return true;
	}

	void CAnimatedMeshNode::addTime(f32 t)
	{
		if (mCurrAnimationId < 0)
			return;

		mTimePos += t;
		mMesh->getRelativeBoneTransforms(mCurrAnimationId, mTimePos, mRelativeBoneTransforms);
	}

	void CAnimatedMeshNode::updateAbsoluteBoneTransforms()
	{
		if (mCurrAnimationId < 0)
			mMesh->getRelativeBoneTransforms(mAbsoluteBoneTransforms);
		else
			mMesh->getAbsoluteBoneTransforms(mRelativeBoneTransforms, mAbsoluteBoneTransforms);
	}
}