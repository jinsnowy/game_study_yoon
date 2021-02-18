/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Mouse.h																				  *
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
#pragma once
#include "../framework.h"

class Mouse
{
	DECLARE_SINGLE(Mouse)
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type mType;
		bool bLeftIsPressed;
		bool bRightIsPressed;
		int mPosX;
		int mPosY;
	public:
		Event()
			:
			mType(Type::Invalid),
			bLeftIsPressed(false),
			bRightIsPressed(false),
			mPosX(0),
			mPosY(0)
		{}
		Event(Type type, const Mouse& parent)
			:
			mType(type),
			bLeftIsPressed(parent.bLeftIsPressed),
			bRightIsPressed(parent.bRightIsPressed),
			mPosX(parent.mPosX),
			mPosY(parent.mPosY)
		{}
		bool IsValid() const
		{
			return mType != Type::Invalid;
		}
		Type GetType() const
		{
			return mType;
		}
		std::pair<int, int> GetPos() const
		{
			return{ mPosX,mPosY };
		}
		int GetPosX() const
		{
			return mPosX;
		}
		int GetPosY() const
		{
			return mPosY;
		}
		bool LeftIsPressed() const
		{
			return bLeftIsPressed;
		}
		bool RightIsPressed() const
		{
			return bRightIsPressed;
		}
	};
public:
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	Mouse::Event Read();
	bool IsEmpty() const
	{
		return mBuffer.empty();
	}
	void Flush();
private:
	friend class Window;
	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnWheelDelta(int x, int y, int delta);
	void TrimBuffer();
private:
	static constexpr unsigned int kBufferSize = 16u;
	int mPosX;
	int mPosY;
	int mWheelDeltaCarry = 0;
	bool bLeftIsPressed = false;
	bool bRightIsPressed = false;
	bool bIsInWindow = false;
	std::queue<Event> mBuffer;
};