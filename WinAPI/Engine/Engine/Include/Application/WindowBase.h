#pragma once
#include "../Macro.h"

template<class Derived>
class WindowBase
{
protected:
	WindowBase() {};
	~WindowBase() {};
public:
	static Derived& Instance()
	{
		static Derived INSTANCE;
		return INSTANCE; 
	}
	WindowBase(const WindowBase&) = delete; 
	WindowBase(const WindowBase&&) = delete;
	WindowBase& operator=(const WindowBase&) = delete; 
	WindowBase& operator=(const WindowBase&&) = delete;
private:
	static constexpr const char* wndClassName = "myEngine";
public:
	static const char* GetName() { return wndClassName; }
};