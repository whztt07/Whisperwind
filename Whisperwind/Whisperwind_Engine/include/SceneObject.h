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
#ifndef _SCENE_OBJECT_H_
#define _SCENE_OBJECT_H_

#include "Util.h"
#include "EngineForwardDeclare.h"
#include "SceneNode.h"
#include "SceneComponent.h"

namespace Engine
{
	class WHISPERWIND_API SceneObject
	{
	protected:
		explicit SceneObject(const Util::Wstring & name) :
		    mName(name)
		{}

		virtual ~SceneObject()
		{}

	public:
		virtual void addToRenderQueue();
		RenderablePtr & getRenderable(const Util::Wstring & name);
		void updateRenderableFromSceneNode(const SceneNodePtr & sn);

		void preUpdate(Util::time elapsedTime);
		void postUpdate(Util::time elapsedTime);
		void regComponent(ComponentType type, const SceneComponentPtr & sceneComp);
		bool getComponent(ComponentType type, SceneComponentPtr & scene) const;
		void unRegComponent(ComponentType type);

	public:
		GET_CONST_VALUE(Util::Wstring, Name);
		GET_VALUE(SceneNodePtr, AttachedSceneNode);
		SET_GET_CONST_VALUE(Util::AABBPtr, AABB);

	protected:
		void addRenderable(const Util::Wstring & name, const RenderablePtr & renderable);

	private:
		virtual void preUpdate_impl(Util::time elapsedTime) = 0;
		virtual void postUpdate_impl(Util::time elapsedTime) = 0;

	private:
		/// TODO:Sorry for finally I used friend.And if I find a way not to use it,I'll modify it!
 		friend void SceneNode::attachSceneObject(SceneObjectPtr & sceneObj);
 		friend void SceneNode::dettachSceneObject(SceneObjectPtr & sceneObj);
 		SET_VALUE(SceneNodePtr, AttachedSceneNode);

	protected:
		RenderableMap mRenderableMap;

		/// Just do an enum hack!
		SceneComponentPtr mSceneComponents[CT_MAX];
		SceneNodePtr mAttachedSceneNode;
		Util::Wstring mName;
		Util::AABBPtr mAABB;

	private:
		DISALLOW_COPY_AND_ASSIGN(SceneObject);
	};

}

#endif