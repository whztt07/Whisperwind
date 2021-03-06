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

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "DebugDefine.h"
#include "CheckedCast.h"
#include "D3D9ForwardDeclare.h"
#include "D3D9Helper.h"
#include "D3D9RenderTexture.h"
#include "D3D9RenderTarget.h"
#include "D3D9FormatMapping.h"
#include "D3D9Renderable.h"

namespace Engine
{
	//---------------------------------------------------------------------
	void D3D9Renderable::setEffect_impl(const Util::String & effectName)
	{
		mEffect = D3D9Helper::createD3D9Effect(mD3DDevice, effectName);
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::setTechnique_impl(const Util::String & techName)
	{
		WHISPERWIND_ASSERT(!techName.empty());

		D3DXHANDLE tech = mEffect->GetTechniqueByName(techName.c_str());
		DX_IF_NULL_DEBUG_PRINT(tech);

		mTechnique = tech;
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::setEffectSemanticValue_impl(const Util::String & semanticName, const void * data)
	{
		WHISPERWIND_ASSERT(data != NULL);

		EffectHandleSize ehs = getEffectHandleSize(semanticName);

		DX_IF_FAILED_DEBUG_PRINT(mEffect->SetValue(ehs.Handle, data, ehs.Size));
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::setEffectParamValue_impl(const Util::String & paramName, const void * data)
	{
		WHISPERWIND_ASSERT(data != NULL);

		EffectHandleSize ehs = getEffectHandleSize(paramName);

		DX_IF_FAILED_DEBUG_PRINT(mEffect->SetValue(ehs.Handle, data, ehs.Size));
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::setTexture_impl(const Util::String & paramName, const RenderTexturePtr & texture)
	{
		EffectHandleSize ehs = getEffectHandleSize(paramName);

		D3D9RenderTexturePtr d3d9TexPtr = Util::checkedPtrCast<D3D9RenderTexture>(texture);
		if (d3d9TexPtr)
			DX_IF_FAILED_DEBUG_PRINT(mEffect->SetTexture(ehs.Handle, d3d9TexPtr->getTexture().get()))
		else
			DX_IF_FAILED_DEBUG_PRINT(mEffect->SetTexture(ehs.Handle, NULL));
	}
	//---------------------------------------------------------------------
	EffectHandleSize D3D9Renderable::getEffectHandleSize(const Util::String & paramName)
	{
		EffectHandleSize ehs;
		if (mEffectHandleMap.find(paramName) == mEffectHandleMap.end())
		{
			ehs.Handle = mEffect->GetParameterBySemantic(NULL, paramName.c_str());
			if (NULL == ehs.Handle)
				ehs.Handle = mEffect->GetParameterByName(NULL, paramName.c_str());

			WHISPERWIND_ASSERT(ehs.Handle != NULL);

			D3DXPARAMETER_DESC paramDesc;
			DX_IF_FAILED_DEBUG_PRINT(mEffect->GetParameterDesc(ehs.Handle, &paramDesc));
			ehs.Size = paramDesc.Bytes;

			mEffectHandleMap.insert(EffectHandleMap::value_type(paramName, ehs));
		}
		else
		{
			ehs = mEffectHandleMap[paramName];
		}
		WHISPERWIND_ASSERT((ehs.Handle != 0) && (ehs.Size != 0));

		return ehs;
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::onDeviceLost()
	{
		BOOST_FOREACH(IDirect3DVertexBuffer9Ptr & vertexBuf, mVertexBound.VertexBufferVec)
		{
			vertexBuf.reset();
		}

		mVertexBound.VertexDeclaration.reset();

		mIndexBuffer.reset();
	}
	//---------------------------------------------------------------------
	void D3D9Renderable::onDeviceReset(const D3D9RenderablePtr & newRenderable)
	{
		mVertexBound.VertexBufferVec = newRenderable->getVertexBound().VertexBufferVec;
		mVertexBound.VertexDeclaration = newRenderable->getVertexBound().VertexDeclaration;

		mIndexBuffer = newRenderable->getIndexBuffer();
	}

}