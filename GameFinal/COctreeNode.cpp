#include "stdafx.h"
#include "COctreeNode.h"
#include "IOctreeManager.h"
#include "xnacollision/xnacollision.h"

namespace gf
{

	void COctreeNode::addSceneNode(ISceneNode* node)
	{
		/* �Ѿ�����������ײ�ڵ� */
		if (mTreeHeight == mOctreeManager->getMaxTreeHeight())
		{
			mContainedNodes.push_back(node);
			return;
		}

		const math::SAxisAlignedBox& aabb = node->getAabb();

		f32 mid_x = (mMinCorner.x + mMaxCorner.x) * 0.5f;
		f32 mid_y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
		f32 mid_z = (mMinCorner.z + mMaxCorner.z) * 0.5f;

		E_OCTREE_QUADRANT quadrant = getQuadrant(node);
		
		//	if the aabb intersects the boundaries of any sub-octree
		//  suspend the node directly onto the current octree node
		if (quadrant == EOQ_CURRENT)
			mContainedNodes.push_back(node);
		else
			addSceneNodeToChild(quadrant, node);
	}


	E_OCTREE_QUADRANT COctreeNode::getQuadrant(const ISceneNode* node) const
	{
		const math::SAxisAlignedBox& aabb = node->getAabb();

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
					return EOQ_LEFT_UP_FRONT;
				}
				//�����ȫ�ں��
				else if (aabb.Center.z - aabb.Extents.z > mid_z)
				{
					return EOQ_LEFT_UP_BACK;
				}

			}
			//�����ȫ���²�
			else if (aabb.Center.y + aabb.Extents.y < mid_y)
			{
				//�����ȫ��ǰ��
				if (aabb.Center.z + aabb.Extents.z < mid_z)
				{
					return EOQ_LEFT_DOWN_FRONT;
				}
				//�����ȫ�ں��
				else if (aabb.Center.z - aabb.Extents.z > mid_z)
				{
					return EOQ_LEFT_DOWN_BACK;
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
					return EOQ_RIGHT_UP_FRONT;
				}
				//�����ȫ�ں��
				else if (aabb.Center.z - aabb.Extents.z > mid_z)
				{
					return EOQ_RIGHT_UP_BACK;
				}

			}
			//�����ȫ���²�
			else if (aabb.Center.y + aabb.Extents.y < mid_y)
			{
				//�����ȫ��ǰ��
				if (aabb.Center.z + aabb.Extents.z < mid_z)
				{
					return EOQ_RIGHT_DOWN_FRONT;
				}
				//�����ȫ�ں��
				else if (aabb.Center.z - aabb.Extents.z > mid_z)
				{
					return EOQ_RIGHT_DOWN_BACK;
				}
			}
		}

		return EOQ_CURRENT;
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
			minCorner.y = mMinCorner.y;
			maxCorner.y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
		}
		else
		{
			minCorner.y = (mMinCorner.y + mMaxCorner.y) * 0.5f;
			maxCorner.y = mMaxCorner.y;
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

	void COctreeNode::removeSceneNodes()
	{
		mContainedNodes.clear();
		for (u32 i = 0; i < 8; i++)
		{
			if (mChildrenNodes[i])
				mChildrenNodes[i]->removeSceneNodes();
		}
	}

	void COctreeNode::registerVisibleNodes(const math::SFrustum& frustum) const
	{
		math::SAxisAlignedBox aabb;
		aabb.Center = XMFLOAT3((mMaxCorner.x + mMinCorner.x) * 0.5f,
			(mMaxCorner.y + mMinCorner.y) * 0.5f,
			(mMaxCorner.z + mMinCorner.z) * 0.5f);

		aabb.Extents = XMFLOAT3((mMaxCorner.x - mMinCorner.x) * 0.5f,
			(mMaxCorner.y - mMinCorner.y) * 0.5f,
			(mMaxCorner.z - mMinCorner.z) * 0.5f);

		if (math::IntersectAabbFrustum(aabb, frustum) != math::EIS_OUTSIDE)
		{
			// register all the nodes in the containedNodes list
			for (auto it = mContainedNodes.begin(); it != mContainedNodes.end(); it++)
			{
				(*it)->OnRegisterSceneNode(false);
			}

			// recurse the eight sub-space
			for (u32 i = 0; i < 8; i++)
			{
				if (mChildrenNodes[i])
					mChildrenNodes[i]->registerVisibleNodes(frustum);
			}
		}
	}

	/*
		return a IOctreeNode pointer according to the position of the node
		but it doesn't mean this node is certainly contained in this octree node.
	*/
	IOctreeNode* COctreeNode::getBelongedOctreeNode(const ISceneNode* node)
	{
		E_OCTREE_QUADRANT quadrant = getQuadrant(node);
		if (quadrant == EOQ_CURRENT || !mChildrenNodes[quadrant])
			return this;

		return mChildrenNodes[quadrant]->getBelongedOctreeNode(node);
	}

	bool COctreeNode::removeSceneNode(ISceneNode* node)
	{
		if (!(node->getNodeType() & ESNT_MESH))
			return false;

		E_OCTREE_QUADRANT quadrant = getQuadrant(node);
		if (quadrant == EOQ_CURRENT || !mChildrenNodes[quadrant])
		{
			for (auto it = mContainedNodes.begin(); it != mContainedNodes.end(); it++)
			{
				if (*it == node)
				{
					mContainedNodes.erase(it);
					return true;
				}
			}

			return false;
		}
		return mChildrenNodes[quadrant]->removeSceneNode(node);
	}

}
