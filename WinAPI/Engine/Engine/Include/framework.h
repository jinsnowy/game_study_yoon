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

#define WINDOW (Window::Instance())
#define GETRESOLUTION (Window::Instance().GetResolution())
#define GETDC (Window::Instance().GetWndDC())
#define KEYBOARD (Keyboard::Instance())
#define MOUSE (Mouse::Instance())
#define TIMER (FrameTimer::Instance())
#define SCENE_MANAGER (SceneManager::Instance())

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 헤더 파일
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

// 자료구조
#include <bitset>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>

// 알고리즘
#include <algorithm>

#include "Types/Vec2.h"
#include "Types/Rect.h"
#include "Types/Types.h"
#include "Resource.h"
#include "Macro.h"

using namespace std;
typedef Vec2F Size;
typedef Vec2F Pos;
typedef Vec2I RESOLUTION;

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}

template <typename T>
void Delete_SharedPtr_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		iter->reset();
	}

	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second)
	}

	p.clear();
}