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
#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "Util.h"
#include "EngineForwardDeclare.h"
#include "RenderMappingDefines.h"

namespace Engine
{
	class WHISPERWIND_API RenderSystem
	{
	protected:
		RenderSystem()
		{}

		virtual ~RenderSystem();

	public:
		void addToRenderQueue(const RenderablePtr & renderable);
		void renderScene(Util::time elapsedTime);

		inline void init();
		inline bool beginRendering();
		inline bool render(const RenderablePtr & renderable);
		inline void endRendering();
		inline void present();
		inline void clearFrame(Util::u_int flag, Util::real zValue = 1.0, Util::u_int stencilValue = 0);
		inline bool isPaused();

		inline RenderablePtr createRenderable(const RenderableMappingPtr & rm);
		inline RenderTexturePtr createRenderTexture(const RenderTextureMappingPtr & rtm);
		inline RenderTexturePtr createRenderTextureFromFile(const Util::Wstring & path);
		inline RenderTargetPtr createRenderTarget(const RenderTargetMappingPtr & rtm);

		inline void setRenderTarget(Util::u_int index, const RenderTargetPtr & target);
		inline void setRenderTarget(Util::u_int index, const RenderTexturePtr & texture);
		inline void setBlendFactor(BlendFactor srcFactor, BlendFactor destFactor);
		inline void closeBlend();

	public:
		SET_GET_CONST_VALUE(EngineConfigPtr, EngineConfig);

	private:
		virtual void init_impl() = 0;
		virtual bool beginRendering_impl() = 0;
		virtual bool render_impl(const RenderablePtr & renderable) = 0;
		virtual void endRendering_impl() = 0;
		virtual void present_impl() = 0;
		virtual void clearFrame_impl(Util::u_int flag, Util::real zValue, Util::u_int stencilValue) = 0;
		virtual bool isPaused_impl() = 0;

		virtual RenderablePtr createRenderable_impl(const RenderableMappingPtr & rm) = 0;
		virtual RenderTexturePtr createRenderTexture_impl(const RenderTextureMappingPtr & rtm) = 0;
		virtual RenderTexturePtr createRenderTextureFromFile_impl(const Util::Wstring & path) = 0;
		virtual RenderTargetPtr createRenderTarget_impl(const RenderTargetMappingPtr & rtm) = 0;

		virtual void setRenderTarget_impl(Util::u_int index, const RenderTargetPtr & target) = 0;
		virtual void setRenderTarget_impl(Util::u_int index, const RenderTexturePtr & texture) = 0;
		virtual void setBlendFactor_impl(BlendFactor srcFactor, BlendFactor destFactor) = 0;
		virtual void closeBlend_impl() = 0;

	protected:
		EngineConfigPtr mEngineConfig;

	protected:
		RenderQueuePtr mOpaqueRenderQueue;
		RenderQueuePtr mTransparentRenderQueue;

	private:
		DISALLOW_COPY_AND_ASSIGN(RenderSystem);
	};
}

#endif