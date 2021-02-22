#pragma once
#include <string>
#include "../framework.h"
class Ref
{
private:
	class IdGenerator
	{
	public:
		IdGenerator()
			: rng(rnd()), gen(1, numeric_limits<size_t>::max())
		{}
		size_t Pick()
		{
			return gen(rng);
		}
	private:
		random_device rnd;
		mt19937 rng;
		uniform_int_distribution<size_t> gen;
	};
	static IdGenerator m_IdGen;
	static unordered_map<string, size_t> m_IdHashMap;
	static unordered_map<size_t, size_t> m_IdCountMap;
	size_t PickUniqueId(const string& tagName);
	void RemoveId(const string& tagName);
protected:
	Ref();
	virtual ~Ref() = 0;
protected:
	bool m_bEnable;
	bool m_bLife;
	int m_Ref;
	size_t m_id;
	std::string m_strTag;
public:
	static size_t FindId(const string& strTag)
	{
		auto found = m_IdHashMap.find(strTag);
		if (found == m_IdHashMap.end())
			return 0;
		return found->second;
	}
	void AddRef() { ++m_Ref; }
	int Release()
	{
		RemoveId(m_strTag);
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
	size_t GetId() const { return m_id; }
	std::string GetTag() const { return m_strTag; }
	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetTag(const std::string& tagName);
};
