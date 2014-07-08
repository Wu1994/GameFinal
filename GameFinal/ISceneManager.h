#ifndef __ISCENEMANAGER_H__
#define __ISCENEMANAGER_H__

#include "gfTypes.h"
#include "gfUtil.h"
#include "IReferenceCounted.h"
#include "IDevice.h"
#include "IModelMeshNode.h"
#include "IVideoDriver.h"
#include "IGeometryCreator.h"
#include "IMaterial.h"
#include "IResourceGroupManager.h"
#include "IFpsCameraNode.h"
#include "IAnimatedMeshNode.h"
#include "IOctreeManager.h"
#include "ITerrainNode.h"

namespace gf
{

	class IMeshNode;
	class ILightNode;

	class ISceneManager : public ISceneNode
	{
	public:

	public:
		ISceneManager(IDevice* device)
			:ISceneNode(nullptr, this)
			, mDevice(device)

		{

		}

		virtual void drawAll() = 0;

		virtual void draw(ISceneNode* node) = 0;

		IDevice* getDevice()
		{
			return mDevice;
		}


		virtual void registerNodeForRendering(IMeshNode* node, E_NODE_TYPE nodeType = ENT_SOLID_NODE) = 0;

		virtual ISceneNode* addEmptyNode(
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f)) = 0;

		virtual IMeshNode* addMeshNode(
			ISimpleMesh* mesh,
			IMaterial* material,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f)) = 0;

		virtual IOctreeManager* addOctreeManagerNode(ISceneNode* parent,
			f32 width,
			f32 height,
			f32 depth,
			bool staticOctree = true, 
			XMFLOAT3 center = XMFLOAT3(0, 0, 0),
			u32 maxTreeHeight = 8) = 0;

		virtual IMeshNode* addModelMeshNode(
			IModelMesh* mesh,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f)) = 0;

		virtual IAnimatedMeshNode* addAnimatedMeshNode(
			IAnimatedMesh* mesh,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& rotation = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& scale = XMFLOAT3(1.0f, 1.0f, 1.0f)) = 0;

		virtual ITerrainNode* addTerrainNode(
			ITerrainMesh* mesh,
			IMaterial* material = nullptr,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0)) = 0;

		virtual ILightNode* addLightNode(u32 id,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, 0)) = 0;

		virtual ICameraNode* addCameraNode(u32 id,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, -5.0f),
			const XMFLOAT3& lookat = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& up = XMFLOAT3(0, 1.0f, 0),
			f32 fov = XM_PIDIV4,
			f32 nearZ = 1.0f,
			f32 farZ = 1000.0f,
			f32 aspectRatio = -1.0f) = 0;

		virtual IFpsCameraNode* addFpsCameraNode(u32 id,
			ISceneNode* parent = nullptr,
			const XMFLOAT3& position = XMFLOAT3(0, 0, -5.0f),
			const XMFLOAT3& lookat = XMFLOAT3(0, 0, 0),
			const XMFLOAT3& up = XMFLOAT3(0, 1.0f, 0),
			f32 maxUpAngle = XM_PI * 0.33f,
			f32 maxDownAngle = XM_PI * 0.33f,
			f32 fov = XM_PIDIV4,
			f32 nearZ = 1.0f,
			f32 farZ = 1000.0f,
			f32 aspectRatio = -1.0f) = 0;

		virtual ILightNode* getLightNode(u32 id) = 0;

		

		IVideoDriver* getVideoDriver()
		{
			return mVideoDriver;
		}

		virtual ICameraNode* getActiveCameraNode() = 0;

		virtual u32 getActiveCameraId() const = 0;

		virtual ICameraNode* getCameraNode(u32 id) = 0;

		virtual ICameraNode* setActiveCamera(u32 id) = 0;

		virtual bool setActiveCamera(ICameraNode* camera) = 0;

		virtual u32 getRenderedMeshNum() const = 0;

		virtual E_SCENE_NODE_TYPE getNodeType() const
		{
			return ESNT_SCENE_MANAGER;
		}

	public:
		const static int MAX_LIGHT_COUNT = 1024;
		const static int MAX_CAMERA_COUNT = 16;
		const static int EMPTY_CAMERA_ID = 64;

	protected:
		IDevice*				mDevice;
		IVideoDriver*			mVideoDriver;
	};

}

#endif
