#ifndef __MODEL_MESH_CLASS_H__
#define __MODEL_MESH_CLASS_H__

#include "IModelMesh.h"
namespace gf
{
	class CModelMesh : public IModelMesh
	{
	public:
		CModelMesh(const std::string& name,
			u32 sortcode,
			const math::SAxisAlignedBox& aabb,
			u32	vertexFormat,
			const std::vector<SModelSubset>& subsets,
			IMeshBuffer* buffer)
			:IModelMesh(name, sortcode, aabb, vertexFormat)
			, mMeshBuffer(buffer)
			, mSubsets(subsets)
		{
			for (u32 i = 0; i < mSubsets.size(); i++)
			{
				IMaterial* material = mSubsets[i].Material;
				AddReferenceCounted(material);
			}
		}

		virtual void bind();

		virtual void drawSubset(u32 index) const;

		virtual void bind(IMeshBuffer* pInstanceBuffer);

		virtual void drawSubsetInstanced(u32 subsetIndex, u32 instanceCount) const;

		virtual u32 getSubsetCount() const
		{
			return mSubsets.size();
		}

		virtual IMaterial* getMaterial(u32 index) const
		{
			if (index >= mSubsets.size())
				return nullptr;

			return mSubsets[index].Material;
		}

		virtual bool getMaterialName(u32 index, std::string& name) const
		{
			if (index >= mSubsets.size())
				return false;

			IMaterial* material = mSubsets[index].Material;
			if (!material)
				return false;

			name = material->getName();
			return true;
		}

		virtual E_MESH_TYPE getType() const
		{
			return EMT_MODEL_MESH;
		}

		virtual XMFLOAT4X4 getSubsetTransform(u32 subset) const
		{
			return mSubsets[subset].OffsetMatrix;
		}

		virtual ~CModelMesh()
		{
			ReleaseReferenceCounted(mMeshBuffer);
			for (u32 i = 0; i < mSubsets.size(); i++)
			{
				IMaterial* material = mSubsets[i].Material;
				if (material)
					material->drop();
			}
		}

		virtual IRenderableBuffer* getRenderableBuffer()
		{
			return mMeshBuffer;
		}

	protected:
		IMeshBuffer*							mMeshBuffer;
		std::vector<SModelSubset>				mSubsets;
	};
}

#endif

