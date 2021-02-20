#pragma once

class Ref
{
protected:
	Ref();
	virtual ~Ref() = 0;
protected:
	bool m_bEnable;
	bool m_bLife;
	int m_Ref;
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
	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
};
