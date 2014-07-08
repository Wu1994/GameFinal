#include "stdafx.h"
#include "CModelMeshNode.h"
#include "ISceneManager.h"
#include "CShaderVariableInjection.h"

namespace gf
{

	bool CModelMeshNode::setMaterial(IMaterial* material, u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return false;

		mMaterials[subset] = material;
		return true;
	}

	IMaterial* CModelMeshNode::getMaterial(u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return nullptr;

		return mMaterials[subset];
	}

	bool CModelMeshNode::setMaterialName(const std::string& name, u32 subset)
	{
		if (subset >= mMesh->getSubsetCount())
			return false;

		IMaterial* material = mSceneManager->getVideoDriver()->getMaterialManager()->get(name);
		if (material == nullptr)
			return false;

		mMaterials[subset] = material;
		return true;
	}

	void CModelMeshNode::render()
	{
		mMesh->bind();

		IPipeline* prePipeline = nullptr; /* ��¼ǰһ����ˮ�� */
		IMaterial* preMaterial = nullptr; /* ��¼ǰһ���Ӽ��Ĳ��� */

		u32 subsetCount = mMesh->getSubsetCount();

		for (u32 i = 0; i < subsetCount; i++)
		{
			IMaterial* material = mMaterials[i];
			if (!material)
				continue;


			for (u32 j = 0; j < material->getPipelineCount(); j++)
			{
				IPipeline* pipeline = material->getPipeline(j);
				if (!pipeline)
					continue;

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
				mMesh->drawSubset(i);
			}

			preMaterial = material;
		}

	}

	void CModelMeshNode::OnRegisterSceneNode(bool bRecursion)
	{
		if (mVisible)
		{
			calcSortCode();
			mSceneManager->registerNodeForRendering(this);
			if (bRecursion)
				ISceneNode::OnRegisterSceneNode();
		}
	}

	void CModelMeshNode::calcSortCode()
	{
		/* mesh - 8 bit
		pipeline - 48 bit
		material-texture - 8 bit
		*/
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

}

