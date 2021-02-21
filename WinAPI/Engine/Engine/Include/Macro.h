#pragma once

// 매크로 함수
#define SAFE_DELETE(p)			if(p)	{delete p; p = nullptr;}
#define SAFE_DELETE_ARRAY(p)	if(p)	{delete[] p; p = nullptr;}
#define SAFE_RELEASE(p)			if(p)	{p->Release(); p = nullptr;}

#define KEYDOWN(key)	(Input::Instance().KeyDown(key))
#define KEYUP(key)		(Input::Instance().KeyUp(key))
#define KEYPRESS(key)	(Input::Instance().KeyPress(key))
// #define KEYON(key)		(KEYDOWN(key) || KEYPRESS(key))

#define DEFINITION_SINGLE(Type) Type Type::INSTANCE;
#define DECLARE_SINGLE(Type)	\
		private:\
			static Type INSTANCE;\
			Type();\
			~Type();\
		public:\
			static Type& Instance()\
			{\
				return INSTANCE;\
			}\
			Type(const Type&) = delete;\
			Type(const Type&&) = delete;\
			Type& operator=(const Type&) = delete;\
			Type& operator=(const Type&&) = delete;