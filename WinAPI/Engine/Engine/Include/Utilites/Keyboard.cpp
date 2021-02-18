#include "Keyboard.h"
#include <Windows.h>

DEFINITION_SINGLE(Keyboard)

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

void Keyboard::OnKeyPressed(unsigned char key)
{
	mKeyState[key] = true;
}

void Keyboard::OnKeyReleased(unsigned char key)
{
	mKeyState[key] = false;
}

void Keyboard::OnChar(char character) 
{
	mCharBuffer.push(character);
	TrimBuffer(mCharBuffer);
}

void Keyboard::ClearState()
{
	mKeyState.reset();
}

bool Keyboard::IsKeyPressed(unsigned char key)
{
	// real time check
	return mKeyState[key] || (GetAsyncKeyState(key) & 0x8000);
}

Keyboard::Event Keyboard::ReadKey() 
{
	if (mKeyBuffer.size() > 0u)
	{
		Keyboard::Event e = mKeyBuffer.front();
		mKeyBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const 
{
	return mKeyBuffer.empty();
}

char Keyboard::ReadChar() 
{
	if (mCharBuffer.size() > 0u)
	{
		unsigned char charcode = mCharBuffer.front();
		mCharBuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const 
{
	return mCharBuffer.empty();
}

void Keyboard::FlushKey() 
{
	mKeyBuffer = std::queue<Event>();
}

void Keyboard::FlushChar() 
{
	mCharBuffer = std::queue<char>();
}

void Keyboard::Flush() 
{
	FlushKey();
	FlushChar();
}

void Keyboard::SetRepeatedInputCapture(bool repeat)
{
	bRepeatedInputCapture = repeat;
}

bool Keyboard::EnabledRepeatedInputCapture() const
{
	return bRepeatedInputCapture;
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > kBufferSize)
	{
		buffer.pop();
	}
}