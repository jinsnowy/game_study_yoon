// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// 매크로
#define WM_RENDER_RESET (WM_USER + 1)
#define SAFE_DELETE(p)			if(p)	{delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p)	{delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)			if(p)	{p->Release(); p = nullptr;}

#define DECLARE_SINGLE(Type)	\
		private:\
			Type();\
			~Type();\
		public:\
			static Type& Instance()\
			{\
				static Type INSTANCE;\
				return INSTANCE;\
			}\
			Type(const Type&) = delete;\
			Type(const Type&&) = delete;\
			Type& operator=(const Type&) = delete;\
			Type& operator=(const Type&&) = delete;

#define KEYBOARD (Keyboard::Instance())
#define MOUSE (Mouse::Instance())
#define TIMER (FrameTimer::Instance())

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 헤더 파일
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <bitset>
#include <queue>


