/*-------------------------------------------------------------------------
This source file is a part of Whisperwind(GameEngine + GamePlay + GameTools)
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
#include "ApplicationCofig.h"

#if USE_VLD != 0
  /** Add for visual leak detector. */
  #include "Visual Leak Detector/include/vld.h"
#endif

/** for windows.h's warning level */
#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)

#include "GamePlay.h"

/** In this project,the main purpose is trying to run the game with any launchers,
      and of course, here you can do many things such as crash dump generation and 
	  dump collection to the server and so on.
*/
Util::s_int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, Util::s_int)
{
	try
	{
		GamePlay::GamePlayFramework framework(APPLICATION_NAME);
		framework.run();
	}
	catch (Util::Exception & e)
	{
#ifdef WHISPERWIND_DEBUG
		::MessageBoxA(NULL, boost::diagnostic_information_what(e), "Error!", MB_OK);
#else
		/// For compiler happy.
		/// TODO:Need replace with a log!
		e.what();
		::MessageBox(NULL, ERROR_NOTIFY.c_str(), TO_UNICODE("Error!"), MB_OK);		
#endif
	}

	return 0;
}