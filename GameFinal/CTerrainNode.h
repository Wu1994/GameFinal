#ifndef __TERRAIN_NODE_CLASS_H__
#define __TERRAIN_NODE_CLASS_H__

#include "ITerrainNode.h"
#include "ITerrainMesh.h"
#include "IMaterial.h"

namespace gf
{
	class CTerrainNode : public ITerrainNode
	{
	public:
		CTerrainNode(ISceneNode* parent,
			ISceneManager* smgr,
			ITerrainMesh* mesh,
			IMaterial* material = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f))
			:ITerrainNode(parent, smgr, position, rotation, scale)
			, mMesh(mesh)
			, mMaterial(material)
		{
			AddReferenceCounted(mesh);
			AddReferenceCounted(mMaterial);
		}

		virtual ~CTerrainNode()
		{
			ReleaseReferenceCounted(mMesh);
			ReleaseReferenceCounted(mMaterial);
		}

		virtual void render();

		virtual void OnRegisterSceneNode(bool bRecursion = true);

		virtual bool setMaterialName(const std::string& name, u32 subset = 0);

		virtual void calcSortCode();

		virtual bool setMaterial(IMaterial* material, u32 subset = 0)
		{
			ReleaseReferenceCounted(mMaterial);
			mMaterial = material;
			AddReferenceCounted(mMaterial);
			return true;
		}

		virtual IMaterial* getMaterial(u32 subset = 0)
		{
			return mMaterial;
		}

		virtual u32 getSubsetCount() const
		{
			return 1;
		}

		virtual void updateAbsoluteTransformation()
		{
			ISceneNode::updateAbsoluteTransformation();

			const math::SAxisAlignedBox& aabb = mMesh->getAabb();
			getLocalAxis(mOBB.Axis);
			mOBB.Center = getAbsolutePosition();
			
			mOBB.Center.x += aabb.Center.x;
			mOBB.Center.y += aabb.Center.y;
			mOBB.Center.z += aabb.Center.z;

			mOBB.Extents = aabb.Extents;
		}

		virtual E_SCENE_NODE_TYPE getNodeType() const
		{
			return ESNT_TERRAIN_MESH;
		}

		virtual ITerrainMesh* getTerrainMesh()
		{
			return mMesh;
		}

	private:
		ITerrainMesh*			mMesh;
		IMaterial*				mMaterial;

	};
}

#endif

