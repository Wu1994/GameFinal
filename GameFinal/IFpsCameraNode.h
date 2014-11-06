#ifndef __FPS_CAMERA_NODE_INTERFACE_H__
#define __FPS_CAMERA_NODE_INTERFACE_H__

#include "ICameraNode.h"

namespace gf
{

	class IFpsCameraNode : public ICameraNode
	{
	public:
		IFpsCameraNode(ISceneNode* parent, ISceneManager* smgr,
			const XMFLOAT3& position,
			f32 aspectRatio,
			f32 fov,
			f32 nearZ,
			f32 farZ,
			f32 maxUpAngle,
			f32 maxDownAngle,
			bool bPerspectiveProj)
			:ICameraNode(parent, smgr, position, aspectRatio, fov, nearZ, farZ, bPerspectiveProj)
			, mMaxUpAngle(maxUpAngle)
			, mMaxDownAngle(maxDownAngle)
		{

		}

		virtual E_SCENE_NODE_TYPE getNodeType() const
		{
			return ESNT_FPS_CAMERA;
		}

	protected:
		f32								mMaxUpAngle;			/* ���̧ͷ�Ƕ� */
		f32								mMaxDownAngle;			/* ����ͷ�Ƕ� */
	};

}

#endif
