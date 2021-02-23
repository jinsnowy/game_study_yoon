#pragma once
#include <exception>
#include <string>

using namespace std;

class UserException : public exception
{
protected:
	mutable wstring m_whatBuffer;
private:
	int erCodeLine;
	string erFileName;
public:
	UserException(int codeLine, const char* fileName) noexcept;
	// 예외 발생시 상세 내용 출력 인터페이스 
	virtual const wchar_t* What() const noexcept final;
	virtual const wchar_t* GetType() const noexcept;
	virtual wstring GetErrorMessage() const noexcept;
};
