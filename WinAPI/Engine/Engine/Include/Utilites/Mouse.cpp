/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Mouse.cpp																			  *
 *	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "Mouse.h"
#include <Windows.h>

DEFINITION_SINGLE(Mouse)

Mouse::Mouse()
	: mPosX(0), mPosY(0)
{

}

Mouse::~Mouse()
{

}

std::pair<int, int> Mouse::GetPos() const
{
	return { mPosX,mPosY };
}

int Mouse::GetPosX() const
{
	return mPosX;
}

int Mouse::GetPosY() const
{
	return mPosY;
}

bool Mouse::IsInWindow() const
{
	return bIsInWindow;
}

bool Mouse::LeftIsPressed() const
{
	return bLeftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return bRightIsPressed;
}

Mouse::Event Mouse::Read()
{
	if (mBuffer.size() > 0u)
	{
		Mouse::Event e = mBuffer.front();
		mBuffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::Flush()
{
	mBuffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy)
{
	mPosX = newx;
	mPosY = newy;

	mBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	bIsInWindow = false;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	bIsInWindow = true;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int mPosX, int mPosY)
{
	bLeftIsPressed = true;

	mBuffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int mPosX, int mPosY)
{
	bLeftIsPressed = false;

	mBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int mPosX, int mPosY)
{
	bRightIsPressed = true;

	mBuffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int mPosX, int mPosY)
{
	bRightIsPressed = false;

	mBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int mPosX, int mPosY)
{
	mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int mPosX, int mPosY)
{
	mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (mBuffer.size() > kBufferSize)
	{
		mBuffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	// 휠이 일정한 만큼 돌아야 OnWheelUp과 OnWheelDown을 호출
	mWheelDeltaCarry += delta;
	// generate events for every 120 
	while (mWheelDeltaCarry >= WHEEL_DELTA)
	{
		mWheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (mWheelDeltaCarry <= -WHEEL_DELTA)
	{
		mWheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}