#pragma once
#include <exception>
#include <string>

class UserException : public std::exception
{
protected:
	mutable std::string mWhatBuffer;
private:
	int erCodeLine;
	std::string erFileName;
public:
	UserException(int codeLine, const char* fileName) noexcept;
	// ���� �߻��� �� ���� ��� �������̽� 
	virtual const char* what() const noexcept final;
	virtual const char* GetType() const noexcept;
	virtual std::string GetErrorMessage() const noexcept;
};
