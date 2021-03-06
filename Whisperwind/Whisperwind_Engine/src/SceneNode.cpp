/*-------------------------------------------------------------------------
This source file is a part of Whisperwind.(GameEngine + GamePlay + GameTools)
For the latest info, see http://lisuyong.com

Copyright (c) 2012 Suyong Li (79481268@qq.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

#include <algorithm>
#include <boost/typeof/typeof.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "DebugDefine.h"
#include "AABB.h"
#include "SceneObject.h"
#include "SceneManager.h"
#include "EngineManager.h"
#include "SceneNode.h"

namespace Engine
{
	static const SceneNodePtr NULL_SCENE_NODE;
	//---------------------------------------------------------------------
	SceneNode::SceneNode(const Util::Wstring & name, Util::u_int nodeType) :
	    mName(name),
		mPosition(0.0f, 0.0f, 0.0f),
		mRelativePosition(0.0f, 0.0f, 0.0f),
		mNodeType(nodeType)
	{
		XMStoreFloat4(&mOrientation, XMQuaternionIdentity());
		XMStoreFloat4(&mRelativeOrientation, XMQuaternionIdentity());

		mAABB = boost::make_shared<Util::AABB>();
	}
	//---------------------------------------------------------------------
	SceneNode::~SceneNode()
	{
		dettachAllSceneObject();
		removeAllChildNode();
	}
	//---------------------------------------------------------------------
	void SceneNode::attachSceneObject(SceneObjectPtr & sceneObj)
	{
		WHISPERWIND_ASSERT(std::find(mSceneObjectVec.begin(), mSceneObjectVec.end(), sceneObj) == mSceneObjectVec.end());

		if (sceneObj->getAttachedSceneNode())
			sceneObj->getAttachedSceneNode()->dettachSceneObject(sceneObj);

		mSceneObjectVec.push_back(sceneObj);

		sceneObj->setAttachedSceneNode(this->shared_from_this());

		/// calc AABB
		mergeAABBFromSceneObject(sceneObj);
	}
	//---------------------------------------------------------------------
	void SceneNode::dettachSceneObject(SceneObjectPtr & sceneObj)
	{
		WHISPERWIND_ASSERT(std::find(mSceneObjectVec.begin(), mSceneObjectVec.end(), sceneObj) != mSceneObjectVec.end());

		sceneObj->setAttachedSceneNode(NULL_SCENE_NODE);

		mSceneObjectVec.erase(std::find(mSceneObjectVec.begin(), mSceneObjectVec.end(), sceneObj));

		reCalcAABB();
	}
	//---------------------------------------------------------------------
	void SceneNode::dettachAllSceneObject()
	{
		while (!mSceneObjectVec.empty())
		{
			dettachSceneObject(*(mSceneObjectVec.begin()));
		}

		mSceneObjectVec.clear();
	}
	//---------------------------------------------------------------------
	SceneNodePtr & SceneNode::createChildNode(const Util::Wstring & name)
	{
		SceneNodePtr & sceneNode = EngineManager::getSingleton().getSceneManager()->createSceneNode(name, NT_AS_CHILD | mNodeType);

		sceneNode->setParentNode(this->shared_from_this());

		return sceneNode;
	}
	//---------------------------------------------------------------------
	void SceneNode::addChildNode(const SceneNodePtr & childNode)
	{
		WHISPERWIND_ASSERT(std::find(mChildSceneNodeVec.begin(), mChildSceneNodeVec.end(), childNode) == mChildSceneNodeVec.end());

		mChildSceneNodeVec.push_back(childNode);
	}
	//---------------------------------------------------------------------
	void SceneNode::setParentNode(const SceneNodePtr & parentNode)
	{
		SceneNodePtr me = this->shared_from_this();
		if (mParentNode)
		{
			mParentNode->removeChildNode(me);
			mParentNode.reset();
		}

		if (parentNode)
			parentNode->addChildNode(me);

		mParentNode = parentNode;
	}
	//---------------------------------------------------------------------
	void SceneNode::removeChildNode(SceneNodePtr & childNode)
	{
		BOOST_AUTO(childIt, std::find(mChildSceneNodeVec.begin(), mChildSceneNodeVec.end(), childNode));
		if (childIt != mChildSceneNodeVec.end())
		{
			mChildSceneNodeVec.erase(childIt);
			childNode->setParentNode(NULL_SCENE_NODE);
		}
	}
	//---------------------------------------------------------------------
	void SceneNode::removeAllChildNode()
	{
		while (!mChildSceneNodeVec.empty())
		{
			removeChildNode(*(mChildSceneNodeVec.begin()));
		}
		mChildSceneNodeVec.clear();
	}
	//---------------------------------------------------------------------
	bool SceneNode::getChildNode(const Util::Wstring & name, SceneNodePtr & outChildNode) const
	{
		for (SceneNodeVector::const_iterator childIt; childIt != mChildSceneNodeVec.end(); ++ childIt)
		{
			if (name == (*childIt)->getName())
			{
				outChildNode = *childIt;
				return true;
			}
		}

		return false;
	}
	//---------------------------------------------------------------------
	bool SceneNode::getParentNode(SceneNodePtr & outParentNode) const
	{
		if (mParentNode)
		{
			outParentNode = mParentNode;
			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	void SceneNode::addToRenderQueue()
	{
		BOOST_FOREACH(const SceneObjectPtr & so, mSceneObjectVec)
		{
			so->addToRenderQueue();
		}
	}
	//---------------------------------------------------------------------
	XMVECTOR SceneNode::getPosition() const
	{
		if (mParentNode)
			return mParentNode->getPosition() + XMLoadFloat3(&mRelativePosition);
		else
			return XMLoadFloat3(&mPosition);
	}
	//---------------------------------------------------------------------
	void SceneNode::setPosition(FXMVECTOR position)
	{
		mAABB->move(position - XMLoadFloat3(&mPosition));

		XMStoreFloat3(&mPosition, position);

		if (mParentNode)
			XMStoreFloat3(&mRelativePosition, position - mParentNode->getPosition());

		setNeedUpdateChilds();
	}
	//---------------------------------------------------------------------
	XMVECTOR SceneNode::getRelativePosition() const
	{
		WHISPERWIND_ASSERT(mParentNode != NULL);

		return XMLoadFloat3(&mRelativePosition);
	}
	//---------------------------------------------------------------------
	void SceneNode::setRelativePosition(FXMVECTOR relPosition)
	{
		WHISPERWIND_ASSERT(mParentNode != NULL);

		mAABB->move(relPosition - XMLoadFloat3(&mRelativePosition));

		XMStoreFloat3(&mRelativePosition, relPosition);

		if (mParentNode)
			XMStoreFloat3(&mPosition, relPosition + mParentNode->getPosition());

		setNeedUpdateChilds();
	}
	//---------------------------------------------------------------------
	XMVECTOR SceneNode::getOrientation() const
	{
		if (mParentNode)
			return XMQuaternionMultiply(XMLoadFloat4(&mRelativeOrientation), mParentNode->getOrientation());
		else
			return XMLoadFloat4(&mOrientation);
	}
	//---------------------------------------------------------------------
	void SceneNode::setOrientation(FXMVECTOR orientation)
	{
		mAABB->rotate(XMQuaternionMultiply(XMQuaternionInverse(XMLoadFloat4(&mOrientation)), orientation));

		XMStoreFloat4(&mOrientation, orientation);

		if (mParentNode)
			XMStoreFloat4(&mRelativeOrientation, XMQuaternionMultiply(XMQuaternionInverse(mParentNode->getOrientation()), orientation));

		setNeedUpdateChilds();
	}
	//---------------------------------------------------------------------
	XMVECTOR SceneNode::getRelativeOrientation() const
	{
		WHISPERWIND_ASSERT(mParentNode != NULL);

		return XMLoadFloat4(&mRelativeOrientation);
	}
	//---------------------------------------------------------------------
	void SceneNode::setRelativeOrientation(FXMVECTOR relOrientation)
	{
		WHISPERWIND_ASSERT(mParentNode != NULL);

		mAABB->rotate(XMQuaternionMultiply(XMQuaternionInverse(XMLoadFloat4(&mRelativeOrientation)), relOrientation));

		XMStoreFloat4(&mRelativeOrientation, relOrientation);

		if (mParentNode)
			XMStoreFloat4(&mOrientation, XMQuaternionMultiply(mParentNode->getOrientation(), relOrientation));

		setNeedUpdateChilds();
	}
	//---------------------------------------------------------------------
	void SceneNode::move(FXMVECTOR trans)
	{
		if (!mParentNode)
			setPosition(XMLoadFloat3(&mPosition) + trans);
		else
			setRelativePosition(XMLoadFloat3(&mRelativePosition) + trans);
	}
	//---------------------------------------------------------------------
	void SceneNode::rotate(FXMVECTOR orientation)
	{
		if (!mParentNode)
			setOrientation(XMQuaternionMultiply(XMLoadFloat4(&mOrientation), orientation));
		else
			setRelativePosition(XMQuaternionMultiply(XMLoadFloat4(&mRelativeOrientation), orientation));
	}
	//---------------------------------------------------------------------
	void SceneNode::setNeedUpdateChilds()
	{
		mNeedUpdateChilds = true;

		if (mParentNode)
			mParentNode->setNeedUpdateChilds();
	}
	//---------------------------------------------------------------------
	void SceneNode::update(Util::time elapsedTime)
	{
		if (mParentNode)
		{
			this->setPosition(mParentNode->getPosition() + XMLoadFloat3(&mRelativePosition));
			this->setOrientation(XMQuaternionMultiply(mParentNode->getOrientation(), XMLoadFloat4(&mRelativeOrientation)));
		}
		
		if (!mNodeTrack.isEmpty())
			updateNodeTrack(elapsedTime);

		if (mNeedUpdateChilds)
		{
			BOOST_AUTO(it, mChildSceneNodeVec.begin());
			for (it; it != mChildSceneNodeVec.end(); ++it)
			{
				(*it)->update(elapsedTime);
			}
		}

		mNeedUpdateChilds = false;
	}
	//---------------------------------------------------------------------
	void SceneNode::mergeAABBFromSceneObject(const SceneObjectPtr & so)
	{
		const Util::AABBPtr & aabb = so->getAABB();
		IF_NULL_RETURN(aabb);

		aabb->move(XMLoadFloat3(&mPosition));
		aabb->rotate(XMLoadFloat4(&mOrientation));

		mAABB->merge(aabb);

		updatedAABB();

		so->updateRenderableFromSceneNode(this->shared_from_this());
	}
	//---------------------------------------------------------------------
	void SceneNode::reCalcAABB()
	{
		mAABB->reset();

		BOOST_FOREACH(const SceneObjectPtr & so, mSceneObjectVec)
		{
			mergeAABBFromSceneObject(so);
		}
	}
	//---------------------------------------------------------------------
	void SceneNode::setAABB(const Util::AABBPtr & aabb)
	{
		IF_FALSE_RETURN(aabb && !aabb->getIsInvalid());

		mAABB = aabb;

		updatedAABB();
	}
	//---------------------------------------------------------------------
	void SceneNode::addTrackPoint(const NodeControllPoint & point)
	{
		mNodeTrack.ControllPointVec.push_back(point);
	}
	//---------------------------------------------------------------------
	void SceneNode::updateNodeTrack(Util::time elapsedTime)
	{
		/// TODO:Orientation.

		if (mNodeTrack.RestTrans <= 0.0f)
		{
			if ((static_cast<Util::s_int>(mNodeTrack.ControllPointVec.size()) - 1) == mNodeTrack.CurrentPoint)
				mNodeTrack.CurrentPoint = 0;
			else
				++mNodeTrack.CurrentPoint;

			const NodeControllPoint & currentPoint = mNodeTrack.ControllPointVec[mNodeTrack.CurrentPoint];

			XMVECTOR deltaTrans = XMLoadFloat3(&currentPoint.Position) - this->getPosition();
			mNodeTrack.RestTrans = XMVectorGetX(XMVector3Length(deltaTrans));
			XMStoreFloat3(&mNodeTrack.CurrentDirection, XMVector3Normalize(deltaTrans));
		}

		Util::real trans = elapsedTime * mNodeTrack.MoveSpeed;

		if (NTM_AS_WORLD == mNodeTrack.TrackMode)
			this->move(trans * XMLoadFloat3(&mNodeTrack.CurrentDirection));

		/// TODO:else if (NTM_AS_PARENT == mNodeTrack.TrackMode)

		mNodeTrack.RestTrans -= trans;

		mNeedUpdateChilds = true;
	}

}
