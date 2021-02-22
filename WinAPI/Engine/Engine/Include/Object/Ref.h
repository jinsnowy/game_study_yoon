#pragma once
#include <string>
class Ref
{
protected:
	Ref();
	virtual ~Ref() = 0;
protected:
	bool m_bEnable;
	bool m_bLife;
	int m_Ref;
	std::string m_strTag;
public:
	void AddRef() { ++m_Ref; }
	int Release()
	{
		--m_Ref;
		if (m_Ref == 0)
		{
			delete this;
			return 0;
		}
		return m_Ref;
	}
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }
	std::string GetTag()const { return m_strTag; }
	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetTag(const std::string& str) { m_strTag = str; }
};
