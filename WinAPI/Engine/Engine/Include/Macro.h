#pragma once


// ¸ÅÅ©·Î
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