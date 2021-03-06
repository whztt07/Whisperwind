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
#ifndef _ENGINE_COFING_H_
#define _ENGINE_COFING_H_

#include "Util.h"
#include "Config.h"

namespace Engine
{
	class WHISPERWIND_API EngineConfig : public Config
	{
	public:
		explicit EngineConfig(const Util::String & name);

		~EngineConfig()
		{}

	public:
		SET_GET_CONST_VALUE(bool, FullScreen);
		SET_GET_CONST_VALUE(bool, VSync);
		SET_GET_CONST_VALUE(bool, PerfHUD);
		SET_GET_CONST_VALUE(bool, DebugRendering);
		SET_GET_CONST_VALUE(bool, DebugNormal);
		SET_GET_CONST_VALUE(bool, DefferdRendering);
		SET_GET_CONST_VALUE(Util::UintPair, ResolutionPair);
		SET_GET_CONST_VALUE(Util::u_int, MultiSampleQuality);
		SET_GET_CONST_VALUE(Util::u_int, MultiSampleType);

	private:
		virtual void parse_impl();

	private:
		bool mFullScreen;
		bool mVSync;
		bool mPerfHUD;
		bool mDebugRendering;
		bool mDebugNormal;
		bool mDefferdRendering;
		Util::UintPair mResolutionPair;
		Util::u_int mMultiSampleQuality;
		Util::u_int mMultiSampleType;
	};
}

#endif