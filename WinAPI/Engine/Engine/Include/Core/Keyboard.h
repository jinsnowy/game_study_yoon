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

class Keyboard
{
	DECLARE_SINGLE(Keyboard)
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code)
			:
			type(type),
			code(code)
		{}
		bool IsPress() const
		{
			return type == Type::Press;
		}
		bool IsRelease() const
		{
			return type == Type::Release;
		}
		bool IsValid() const
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const
		{
			return code;
		}
	};
private:
	static constexpr unsigned int kKeys = 256u;
	static constexpr unsigned int kBufferSize = 16u;
	bool bRepeatedInputCapture = false;
	bitset<kKeys> mKeyState;
	queue<Event> mKeyBuffer;
	queue<char> mCharBuffer;
private:
	friend class Window;
	void ClearState();
	void OnKeyPressed(unsigned char key);
	void OnKeyReleased(unsigned char key);
	void OnChar(char charcter);
	template<typename T>
	static void TrimBuffer(queue<T>& buffer);

	// key event stuff
	bool IsKeyPressed(unsigned char key);
	Event ReadKey();
	bool KeyIsEmpty() const;
	void FlushKey();
	// char event stuff
	char ReadChar();
	bool CharIsEmpty() const;
	void FlushChar();
	void Flush();
	// repeated key input capture mode
	void SetRepeatedInputCapture(bool repeat);
	bool EnabledRepeatedInputCapture() const;
};