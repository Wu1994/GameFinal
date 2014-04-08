#include "stdafx.h"
#include "COctreeNode.h"
#include "IOctreeManager.h"
#include "xnacollision/xnacollision.h"

void COctreeNode::addSceneNode(ISceneNode* node)
{
	/* �Ѿ�����������ײ�ڵ� */
	if (mTreeHeight == mOctreeManager->getMaxTreeHeight())
	{
		mContainedNodes.push_back(node);
		return;
	}

	const XNA::AxisAlignedBox& aabb = node->getAabb();

	f32 mid_x = (mMinCorner.x + mMaxCorner.x) * 0.5f;
	f32 mid_y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
	f32 mid_z = (mMinCorner.z + mMaxCorner.z) * 0.5f;

	//�����ȫ�����
	if (aabb.Center.x + aabb.Extents.x < mid_x)
	{
		//�����ȫ���ϲ�
		if (aabb.Center.y - aabb.Extents.y > mid_y)
		{
			//�����ȫ��ǰ��
			if (aabb.Center.z + aabb.Extents.z < mid_z)
			{
				addSceneNodeToChild(EOQ_LEFT_UP_FRONT, node);
			}
			//�����ȫ�ں��
			else if (aabb.Center.z - aabb.Extents.z > mid_z)
			{
				addSceneNodeToChild(EOQ_LEFT_UP_BACK, node);
			}

		}
		//�����ȫ���²�
		else if (aabb.Center.y + aabb.Extents.y < mid_y)
		{
			//�����ȫ��ǰ��
			if (aabb.Center.z + aabb.Extents.z < mid_z)
			{
				addSceneNodeToChild(EOQ_LEFT_DOWN_FRONT, node);
			}
			//�����ȫ�ں��
			else if (aabb.Center.z - aabb.Extents.z > mid_z)
			{
				addSceneNodeToChild(EOQ_LEFT_DOWN_BACK, node);
			}
		}
	}
	//�����ȫ���Ҳ�
	else if (aabb.Center.x - aabb.Extents.x > mid_x)
	{
		//�����ȫ���ϲ�
		if (aabb.Center.y - aabb.Extents.y > mid_y)
		{
			//�����ȫ��ǰ��
			if (aabb.Center.z + aabb.Extents.z < mid_z)
			{
				addSceneNodeToChild(EOQ_RIGHT_UP_FRONT, node);
			}
			//�����ȫ�ں��
			else if (aabb.Center.z - aabb.Extents.z > mid_z)
			{
				addSceneNodeToChild(EOQ_RIGHT_UP_BACK, node);
			}

		}
		//�����ȫ���²�
		else if (aabb.Center.y + aabb.Extents.y < mid_y)
		{
			//�����ȫ��ǰ��
			if (aabb.Center.z + aabb.Extents.z < mid_z)
			{
				addSceneNodeToChild(EOQ_RIGHT_DOWN_FRONT, node);
			}
			//�����ȫ�ں��
			else if (aabb.Center.z - aabb.Extents.z > mid_z)
			{
				addSceneNodeToChild(EOQ_RIGHT_DOWN_BACK, node);
			}
		}
	}
}



void COctreeNode::addSceneNodeToChild(E_OCTREE_QUADRANT quadrant, ISceneNode* node)
{
	if (!mChildrenNodes[quadrant])
	{
		mChildrenNodes[quadrant] = createChildNode(quadrant);
	}

	mChildrenNodes[quadrant]->addSceneNode(node);
}

IOctreeNode* COctreeNode::createChildNode(E_OCTREE_QUADRANT quadrant) const
{
	XMFLOAT3 minCorner, maxCorner;
	// ��������Ҳ������
	if (quadrant & 0x04)
	{
		minCorner.x = (mMinCorner.x + mMaxCorner.x) * 0.5f;
		maxCorner.x = mMaxCorner.x;
	}
	else
	{
		minCorner.x = mMinCorner.x;
		maxCorner.x = (mMinCorner.x + mMaxCorner.x) * 0.5f;
	}

	// ��������²������
	if (quadrant & 0x02)
	{
		minCorner.y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
		maxCorner.y = mMaxCorner.y;
	}
	else
	{
		minCorner.y = mMinCorner.y;
		maxCorner.y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
	}

	// ����Ǳ��������
	if (quadrant & 0x01)
	{
		maxCorner.z = mMaxCorner.z;
		minCorner.z = (mMaxCorner.z + mMinCorner.z) * 0.5f;
	}
	else
	{
		maxCorner.z = (mMaxCorner.z + mMinCorner.z) * 0.5f;
		minCorner.z = mMinCorner.z;
	}

	IOctreeNode* octreeNode = new COctreeNode(mOctreeManager, minCorner, maxCorner, mTreeHeight + 1);
	return octreeNode;
}


