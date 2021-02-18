// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

#define WM_RENDER_RESET (WM_USER + 1)

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
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <bitset>
#include <queue>

#include "Types/Vec2.h"
#include "Types/Rect.h"
#include "Resource.h"
#include "Macro.h"

typedef Vec2F Size;
typedef Vec2F Pos;

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