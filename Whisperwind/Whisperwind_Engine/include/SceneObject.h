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

namespace Engine
{
	enum ComponentType
	{
		CT_RENDERABLE,
		CT_PHYSICABLE,
		CT_SOUNDABLE,
		CT_MAX
	};

	class WHISPERWIND_API SceneObject
	{
	public:
		explicit SceneObject(const Util::Wstring & name) :
		    mName(name)
		{}

	protected:
		virtual ~SceneObject()
		{}

	public:
		void update(Util::time elapsedTime);
		void regComponent(ComponentType type, const SceneComponentPtr & sceneComp);
		bool getComponent(ComponentType type, SceneComponentPtr & scene);

	public:
		GET_CONST_VALUE(Util::Wstring, Name);

	private:
		virtual void update_impl(Util::time elapsedTime) = 0;

	private:
		/// TODO:Sorry for finally I used friend.And if I find a way not to use it,I'll modify it!
 		friend void SceneNode::attachSceneObject(SceneObjectPtr & sceneObj);
 		friend void SceneNode::dettachSceneObject(SceneObjectPtr & sceneObj);
		friend void SceneNode::dettachAllSceneObject();
 		SET_GET_CONST_VALUE(SceneNodePtr, AttachedSceneNode);

	protected:
		SceneComponentPtr mSceneComponents[CT_MAX];
		SceneNodePtr mAttachedSceneNode;
		Util::Wstring mName;

	private:
		DISALLOW_COPY_AND_ASSIGN(SceneObject);
	};
}

#endif