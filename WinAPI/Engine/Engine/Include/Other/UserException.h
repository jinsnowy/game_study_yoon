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
	// ���� �߻��� �� ���� ��� �������̽� 
	virtual const wchar_t* What() const noexcept final;
	virtual const wchar_t* GetType() const noexcept;
	virtual wstring GetErrorMessage() const noexcept;
};
