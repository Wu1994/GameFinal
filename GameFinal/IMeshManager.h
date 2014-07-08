#ifndef __MESH_MANAGER_INTERFACE_H__
#define __MESH_MANAGER_INTERFACE_H__

#include "ISimpleMesh.h"
#include "IModelMesh.h"
#include "IAnimatedMesh.h"
#include "IAnimatedMeshBuffer.h"
#include "ITerrainMesh.h"

namespace gf
{
	class IMeshManager : public IReferenceCounted
	{
	public:

		virtual IModelMesh* createModelMesh(const std::string& name,
			void* vertices,
			void* indices,
			u32 vertexCount,
			u32 vertexStride,
			u32 indiceCount,
			const math::SAxisAlignedBox& aabb,
			const std::vector<IModelMesh::SModelSubset>& subsets,
			bool bit32Index,
			E_MEMORY_USAGE usage = EMU_STATIC) = 0;

		virtual ISimpleMesh* createSimpleMesh(
			const std::string& name,
			void* vertices,
			void* indices,
			u32 vertexCount,
			u32 vertexStride,
			u32 indiceCount,
			const math::SAxisAlignedBox& aabb,
			bool bit32Index,
			E_MEMORY_USAGE usage = EMU_STATIC) = 0;

		virtual IAnimatedMesh* createAnimatedModelMesh(
			const std::string& name,
			void* vertices,
			void* animateVertices,
			void* indices,
			bool bit32Index,
			u32 vertexCount,
			u32 animateVertexCount,
			u32 indicesCount,
			u32 vertexStride,
			u32 animateVertexStride,
			const math::SAxisAlignedBox& aabb,
			E_MEMORY_USAGE usage,
			const std::vector<SModelBone>& bones,
			const std::vector<SAnimatedModelSubset>& subsets,
			const std::vector<SBoneAnimationClip>& animateClips) = 0;


		virtual ISimpleMesh* createCubeMesh(
			const std::string& name,
			f32 width = 1.0f,
			f32 height = 1.0f,
			f32 depth = 1.0f,
			E_MEMORY_USAGE usage = EMU_STATIC) = 0;

		virtual ISimpleMesh* createPlaneMesh(
			const std::string& name,
			f32 width,
			f32 depth,
			u32 xsegments = 1,
			u32 ysegments = 1,
			f32 uTiles = 1.0f,
			f32 vTiles = 1.0f,
			E_MEMORY_USAGE usage = EMU_STATIC) = 0;

		virtual ITerrainMesh* createTerrainMesh(
			const std::string& name,
			const std::string& szRawFileName,
			f32 vertexSpace,
			f32 heightScale,
			bool bCreateTessellationMesh = false,
			bool bCreateNormal = false,
			f32 fTexcoordScale = 1.0f,
			u32 cellsPerPatch = 64,
			E_MEMORY_USAGE usage = EMU_STATIC) = 0;

		virtual IMesh* getMesh(const std::string& name) = 0;

		virtual ISimpleMesh* getSimpleMesh(const std::string& name) = 0;

		virtual IModelMesh* getModelMesh(const std::string& name) = 0;

		virtual IAnimatedMesh* getAnimatedMesh(const std::string& name) = 0;

		virtual ITerrainMesh* getTerrainMesh(const std::string& name) = 0;


		_DECLARE_RESOURCE_DESTROY_METHODS(IMesh);

	};
}

#endif