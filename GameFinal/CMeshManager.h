#ifndef __MESH_MANAGER_CLASS_H__
#define __MESH_MANAGER_CLASS_H__

#include "IMeshManager.h"
#include "IResourceFactory.h"
#include "CSortCodeAllocator.h"
#include "IGeometryCreator.h"
#include "ITextureManager.h"

namespace gf
{

	class CMeshManager : public IMeshManager
	{
	public:
		CMeshManager(IResourceFactory* pResourceFactory, IGeometryCreator* pGeometryCreator, ITextureManager* pTextureManager);

		virtual ISimpleMesh* createSimpleMesh(
			const std::string& name,
			void* vertices,
			void* indices,
			u32 vertexCount,
			u32 vertexStride,
			u32 indiceCount,
			const math::SAxisAlignedBox& aabb,
			bool bit32Index,
			E_MEMORY_USAGE usage = EMU_STATIC);

		virtual IModelMesh* createModelMesh(const std::string& name,
			void* vertices,
			void* indices,
			u32 vertexCount,
			u32 vertexStride,
			u32 indiceCount,
			const math::SAxisAlignedBox& aabb,
			const std::vector<IModelMesh::SModelSubset>& subsets,
			bool bit32Index,
			E_MEMORY_USAGE usage = EMU_STATIC);

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
			const std::vector<SBoneAnimationClip>& animateClips);


		virtual IMesh* getMesh(const std::string& name);

		virtual ISimpleMesh* createCubeMesh(
			const std::string& name,
			f32 width = 1.0f,
			f32 height = 1.0f,
			f32 depth = 1.0f,
			E_MEMORY_USAGE usage = EMU_STATIC);

		virtual ISimpleMesh* createPlaneMesh(
			const std::string& name,
			f32 width,
			f32 depth,
			u32 xsegments = 1,
			u32 ysegments = 1,
			f32 uTiles = 1.0f,
			f32 vTiles = 1.0f,
			E_MEMORY_USAGE usage = EMU_STATIC);

		virtual ITerrainMesh* createTerrainMesh(
			const std::string& name,
			const std::string& szRawFileName,
			f32 vertexSpace,
			f32 heightScale,
			bool bCreateTessellationMesh = false,
			bool bCreateNormal = false,
			f32 fTexcoordScale = 1.0f,
			u32 cellsPerPatch = 64,
			E_MEMORY_USAGE usage = EMU_STATIC);

		virtual ISimpleMesh* getSimpleMesh(const std::string& name);

		virtual IModelMesh* getModelMesh(const std::string& name);

		virtual IAnimatedMesh* getAnimatedMesh(const std::string& name);

		virtual ITerrainMesh* getTerrainMesh(const std::string& name);

		_DEFINE_RESOURCE_DESTROY_METHODS(CMeshManager, mMeshMap, IMesh);

		/*
		bool destroy(const std::string& name)
		{
			auto it = mMeshMap.find(name);
			if (it == mMeshMap.end()) return false;
			IMesh* object = it->second;
			if (object->getReferenceCount() == 1)
			{
				object->drop();
				mMeshMap.erase(it);
				return true;
			}
			return false;
		}
		
		bool destroy(IMesh* object)
		{
			if (!object)
				return false;

			auto it = mMeshMap.find(object->getName());
			if (it == mMeshMap.end())
				return false;
			if (object->getReferenceCount() == 1)
			{
				object->drop();
				mMeshMap.erase(it);
				return true;
			}
			return false;
		}
		
		void destroyAll()								
		{													
			for (auto it = mMeshMap.begin(); it != mMeshMap.end(); it++)	
				ReleaseReferenceCounted(it->second);				
		}													
		virtual ~CMeshManager()
		{
			destroyAll();
		}
		*/
	private:
		IResourceFactory*					mResourceFactory;
		IGeometryCreator*					mGeometryCreator;
		ITextureManager*					mTextureManager;

		std::map<std::string, IMesh*>		mMeshMap;

		CSortCodeAllocator<255>				mCodeAllocator;

	};

}

#endif
