#pragma once
#include <exception>
#include <string>

using namespace std;

class UserException : public exception
{
protected:
	mutable string m_whatBuffer;
private:
	int erCodeLine;
	string erFileName;
public:
	UserException(int codeLine, const char* fileName) noexcept;
	// ���� �߻��� �� ���� ��� �������̽� 
	virtual const char* what() const noexcept final;
	virtual const char* GetType() const noexcept;
	virtual string GetErrorMessage() const noexcept;
};
