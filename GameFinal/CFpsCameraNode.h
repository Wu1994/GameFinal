#ifndef __FPS_CAMERA_NODE_CLASS_H__
#define __FPS_CAMERA_NODE_CLASS_H__

#include "IFpsCameraNode.h"

namespace gf
{

	class CFpsCameraNode : public IFpsCameraNode
	{
	public:
		CFpsCameraNode(ISceneNode* parent, ISceneManager* smgr,
			const XMFLOAT3& position,
			const XMFLOAT3& lookat,
			const XMFLOAT3& up,
			f32 aspectRatio,
			f32 fov,
			f32 nearZ,
			f32 farZ,
			f32 maxUpAngle,
			f32 maxDownAngle,
			bool bPerspectiveProj)
			:IFpsCameraNode(parent, smgr, position, aspectRatio, fov, nearZ, farZ, maxUpAngle, maxDownAngle, bPerspectiveProj)
		{
			XMVECTOR worldup = XMLoadFloat3(&up);
			worldup = XMVector3Normalize(worldup);
			XMStoreFloat3(&mWorldUp, worldup);

			lookAt(lookat);
		}

		virtual void lookAt(const XMFLOAT3& lookat)
		{
			XMVECTOR eye = XMLoadFloat3(&mPosition);
			XMVECTOR target = XMLoadFloat3(&lookat);
			XMVECTOR look = XMVector3Normalize(target - eye);

			XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
			XMVECTOR right = XMVector3Cross(worldup, look);
			right = XMVector3Normalize(right);

			XMVECTOR dir = XMVector3Cross(right, worldup);
			dir = XMVector3Normalize(dir);

			XMVECTOR angle = XMVector3AngleBetweenNormals(dir, look);

			/* �жϵ�ǰ��̧ͷ���ǵ�ͷ�������̧ͷ����angle��Ϊ��������Ϊ��
			�������look������worldup�����ĵ���жϣ�������Ϊ������Ϊ̧ͷ��
			������Ϊ������Ϊ��ͷ */

			mPitchAngle = XMVectorGetX(angle);
			if (XMVectorGetX(XMVector3Dot(look, worldup)) > 0)
				mPitchAngle = -mPitchAngle;

			adjustPitchAngle();
			XMStoreFloat3(&mWalkDir, dir);
		}

		virtual void lookAt(const XMFLOAT3& lookat, const XMFLOAT3& up)
		{
			lookAt(lookat, mUp);
		}

		virtual void look(const XMFLOAT3& dir, const XMFLOAT3& up)
		{
			look(dir, mUp);
		}

		virtual void look(const XMFLOAT3& d)
		{
			XMVECTOR lookdir = XMLoadFloat3(&d);
			lookdir = XMVector3Normalize(lookdir);
			XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
			XMVECTOR right = XMVector3Cross(worldup, lookdir);
			right = XMVector3Normalize(right);

			XMVECTOR dir = XMVector3Cross(right, worldup);
			dir = XMVector3Normalize(dir);

			XMVECTOR angle = XMVector3AngleBetweenNormals(dir, lookdir);

			/* �жϵ�ǰ��̧ͷ���ǵ�ͷ�������̧ͷ����angle��Ϊ��������Ϊ��
			�������look������worldup�����ĵ���жϣ�������Ϊ������Ϊ̧ͷ��
			������Ϊ������Ϊ��ͷ */

			mPitchAngle = XMVectorGetX(angle);
			if (XMVectorGetX(XMVector3Dot(lookdir, worldup)) > 0)
				mPitchAngle = -mPitchAngle;

			adjustPitchAngle();
			XMStoreFloat3(&mWalkDir, dir);
		}

		virtual void render(E_PIPELINE_USAGE usage)
		{

		}

		virtual void walk(f32 unit)
		{
			mPosition.x += mWalkDir.x * unit;
			mPosition.y += mWalkDir.y * unit;
			mPosition.z += mWalkDir.z * unit;
		}

		virtual void fly(f32 unit)
		{
			mPosition.x += mWorldUp.x * unit;
			mPosition.y += mWorldUp.y * unit;
			mPosition.z += mWorldUp.z * unit;
		}

		virtual void strafe(f32 unit)
		{
			XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
			XMVECTOR dir = XMLoadFloat3(&mWalkDir);
			XMVECTOR right = XMVector3Cross(worldup, dir);

			XMVECTOR pos = XMLoadFloat3(&mPosition);
			pos += right * unit;

			XMStoreFloat3(&mPosition, pos);
		}

		/* rotate along camera's right vector. */
		virtual void pitch(f32 angle)
		{
			mPitchAngle += angle;
			adjustPitchAngle();
		}

		/* rotate along camera's up vector. */
		virtual void yaw(f32 angle)
		{
			XMVECTOR up = XMLoadFloat3(&mWorldUp);
			XMVECTOR dir = XMLoadFloat3(&mWalkDir);

			/* construct the rotate matrix. */
			XMMATRIX matRot = XMMatrixRotationNormal(up, angle);

			dir = XMVector3TransformNormal(dir, matRot);
			dir = XMVector3Normalize(dir);

			XMStoreFloat3(&mWalkDir, dir);
		}

		/* rotate along camera's look vector. */
		virtual void roll(f32 angle)
		{
			/* the roll of fps camera is meaningless. */
		}

		virtual void setOrientation(CXMMATRIX M)
		{

		}

		virtual XMMATRIX getRelativeTransformation() const
		{
			return XMLoadFloat4x4(&mRelativeTransform);
		}

		virtual XMMATRIX getAbsoluteTransformation()
		{
			return XMLoadFloat4x4(&mAbsoluteTransformation);
		}

		virtual void updateAbsoluteTransformation()
		{
			XMMATRIX absoluteMatrix;

			XMVECTOR worldup = XMLoadFloat3(&mWorldUp);
			XMVECTOR dir = XMLoadFloat3(&mWalkDir);
			//dir = XMVector3Normalize(dir);

			XMVECTOR right = XMVector3Cross(worldup, dir);
			right = XMVector3Normalize(right);

			/* rotate along the right vector, to get the look vector. */
			XMMATRIX rotMat = XMMatrixRotationNormal(right, mPitchAngle);
			XMVECTOR look = XMVector3TransformNormal(dir, rotMat);
			look = XMVector3Normalize(look);

			XMVECTOR up = XMVector3Cross(look, right);
			up = XMVector3Normalize(up);

			XMStoreFloat3(&mUp, up);
			XMStoreFloat3(&mLook, look);
			XMStoreFloat3(&mRight, right);

			XMMATRIX R = XMMatrixSet(
				mRight.x, mRight.y, mRight.z, 0.0f,
				mUp.x, mUp.y, mUp.z, 0.0f,
				mLook.x, mLook.y, mLook.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			XMMATRIX T = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
			XMMATRIX relativeMatrix = R * T;

			XMStoreFloat4x4(&mRelativeTransform, relativeMatrix);

			if (mParent)
			{
				absoluteMatrix = XMMatrixMultiply(mParent->getAbsoluteTransformation(), relativeMatrix);
			}
			else {
				absoluteMatrix = relativeMatrix;
			}

			XMStoreFloat4x4(&mAbsoluteTransformation, absoluteMatrix);

			// adjust the dir, to prevent the accumulated error.
			dir = XMVector3Normalize(XMVector3Cross(right, worldup));
			XMStoreFloat3(&mWalkDir, dir);

			/* update the view-matrix, proj-matrix, view-proj-matrix */
			XMMATRIX view;
			XMVECTOR pos = XMLoadFloat3(&mPosition);

			view._11 = mRight.x;
			view._21 = mRight.y;
			view._31 = mRight.z;
			view._41 = -XMVectorGetX(XMVector3Dot(right, pos));

			view._12 = mUp.x;
			view._22 = mUp.y;
			view._32 = mUp.z;
			view._42 = -XMVectorGetX(XMVector3Dot(up, pos));

			view._13 = mLook.x;
			view._23 = mLook.y;
			view._33 = mLook.z;
			view._43 = -XMVectorGetX(XMVector3Dot(look, pos));

			view._14 = 0;
			view._24 = 0;
			view._34 = 0;
			view._44 = 1.0f;

			XMMATRIX proj = calcProjMatrix();

			XMStoreFloat4x4(&mViewMatrix, view);
			XMStoreFloat4x4(&mProjMatrix, proj);
			XMMATRIX viewProj = view * proj;
			XMStoreFloat4x4(&mViewProjMatrix, viewProj);

			XMVECTOR det = XMMatrixDeterminant(viewProj);
			XMMATRIX invViewProj = XMMatrixInverse(&det, viewProj);
			XMStoreFloat4x4(&mInvViewProjMatrix, invViewProj);

			// update frustum
			XMFLOAT4X4 M = mViewProjMatrix;
			mFrustum.LeftPlane.x = M._14 + M._11;
			mFrustum.LeftPlane.y = M._24 + M._21;
			mFrustum.LeftPlane.z = M._34 + M._31;
			mFrustum.LeftPlane.w = M._44 + M._41;

			mFrustum.RightPlane.x = M._14 - M._11;
			mFrustum.RightPlane.y = M._24 - M._21;
			mFrustum.RightPlane.z = M._34 - M._31;
			mFrustum.RightPlane.w = M._44 - M._41;

			mFrustum.BottomPlane.x = M._14 + M._12;
			mFrustum.BottomPlane.y = M._24 + M._22;
			mFrustum.BottomPlane.z = M._34 + M._32;
			mFrustum.BottomPlane.w = M._44 + M._42;

			mFrustum.TopPlane.x = M._14 - M._12;
			mFrustum.TopPlane.y = M._24 - M._22;
			mFrustum.TopPlane.z = M._34 - M._32;
			mFrustum.TopPlane.w = M._44 - M._42;

			mFrustum.NearPlane.x = M._13;
			mFrustum.NearPlane.y = M._23;
			mFrustum.NearPlane.z = M._33;
			mFrustum.NearPlane.w = M._43;

			mFrustum.FarPlane.x = M._14 - M._13;
			mFrustum.FarPlane.y = M._24 - M._23;
			mFrustum.FarPlane.z = M._34 - M._33;
			mFrustum.FarPlane.w = M._44 - M._43;

			// normalize the normal of each plane
			for (u32 i = 0; i < 6; i++)
			{
				XMVECTOR planeVec = XMLoadFloat4(&mFrustum.Planes[i]);
				XMVECTOR recipLength = XMVector3ReciprocalLength(planeVec);
				XMVECTOR normPlaneVec = XMVectorMultiply(planeVec, recipLength);
				XMStoreFloat4(&mFrustum.Planes[i], normPlaneVec);
			}

		}

	private:

		void adjustPitchAngle()
		{
			/* the pitch angle must between -maxUpAngle and maxDownAngle*/

			if (mPitchAngle > mMaxDownAngle)
				mPitchAngle = mMaxDownAngle;
			if (mPitchAngle < -mMaxUpAngle)
				mPitchAngle = -mMaxUpAngle;
		}

		XMFLOAT3					mWorldUp;				/* ����������� */
		XMFLOAT3					mWalkDir;				/* ��������߷��� */
		f32							mPitchAngle;			/* �����Ƕȣ�������ƫ�����߷���ĽǶ� */

		XMFLOAT4X4					mRelativeTransform;		/* �������Եı任������ getRelativeTransformation �����ķ���ֵ. */
	};

}

#endif
