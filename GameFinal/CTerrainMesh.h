#ifndef __TERRAIN_MESH_CLASS_H__
#define __TERRAIN_MESH_CLASS_H__

#include "ITerrainMesh.h"
#include "gfUtil.h"

namespace gf
{
	class CTerrainMesh : public ITerrainMesh
	{
	public:
		CTerrainMesh(const std::string& name,
			u32 sortcode,
			f32 vertexSpace,
			f32 heightScale)
			:ITerrainMesh(name, sortcode,
			vertexSpace, heightScale)
		{

		}

		virtual bool init(const std::string& heightmapFileName,
			const std::string& heigtmapFullPath,
			IResourceFactory* resourceFactory,
			ITextureManager* textureManager,
			bool bCreateTessellationMesh,
			bool bCreateNormal = false,
			f32 fTexcoordScale = 1.0f,
			u32 cellsPerPatch = 64,
			E_MEMORY_USAGE usage = EMU_STATIC);
		

		XMFLOAT3 computeVertexNormal(XMFLOAT3 pos, u32 row, u32 col) const;

		f32 getHeightByRowAndCol(u32 row, u32 col) const;

		XMFLOAT2 getPatchHeightBounds(u32 patchRow, u32 patchCol) const;

		virtual E_MESH_TYPE getType() const
		{
			return EMT_TERRAIN_MESH;
		}
		
		virtual ~CTerrainMesh()
		{
			ReleaseReferenceCounted(mMeshBuffer);
			ReleaseReferenceCounted(mHeightMapTexture);
		}

		virtual void bind();

		virtual void draw() const;

	protected:

		std::vector<f32>			mHeightValues;
		
		IMeshBuffer*				mMeshBuffer;

	};
}

#endif
