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

	void CModelMeshNode::render(E_PIPELINE_USAGE usage)
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
			
			IPipeline* pipeline = material->getPipeline(usage);
			if (!pipeline)
				continue;

			//�����ǰ����ˮ�ߺ���һ����ͬ������˲������������������ע��
			if (pipeline == prePipeline)
			{
				//���ҽ�������Ҳ��ͬ����һ��ʱ��ע��
				if (preMaterial != material)
				{
					CShaderVariableInjection::injectMaterial(material, pipeline);
					pipeline->apply(usage);
				}
			}
			else // ���������ˮ�߲�ͬ��ȫ����������Ҫ����
			{
				CShaderVariableInjection::inject(this, pipeline, i);
				pipeline->apply(usage);
				prePipeline = pipeline;
			}

			mMesh->drawSubset(i);

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
		/* customed order - 8 bit, mesh - 8 bit, pipeline - 48 bit */
		IVideoDriver* driver = mSceneManager->getVideoDriver();
		E_PIPELINE_USAGE usage = driver->getPipelineUsage();


		u32 meshCode = (mMesh) ? mMesh->getSortCode() : 0;
		u64 pipeCode = 0;
		if (mMaterials[0])
		{
			if (mMaterials[0]->getPipeline(usage))
				pipeCode = mMaterials[0]->getPipeline(usage)->getSortCode();
		}
		mSortCode = ((u64)mRenderOrder << 56) | ((u64)meshCode << 48) | pipeCode;
	}

}

