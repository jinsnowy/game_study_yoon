#include "Ref.h"
#include <cassert>
Ref::IdGenerator Ref::m_IdGen;
unordered_map<string, size_t> Ref::m_IdHashMap;
unordered_map<size_t, size_t> Ref::m_IdCountMap;

Ref::Ref() : m_Ref(1), m_id(0), m_bEnable(true), m_bLife(true)
{

}

Ref::~Ref()
{
}

size_t Ref::PickUniqueId(const string& tagName)
{
	size_t id;
	do {
		id = m_IdGen.Pick();
	} while (m_IdCountMap.find(id) != m_IdCountMap.end());

	m_IdHashMap.insert(make_pair(tagName, id));
	m_IdCountMap.insert(make_pair(id, 1));
	return id;
}

void Ref::RemoveId(const string& tagName)
{
	if (m_IdHashMap.size() == 0 || tagName.size() == 0) return;
	auto found = m_IdHashMap.find(tagName);
	if (found == m_IdHashMap.end())
		return;
	size_t id = found->second;
	size_t& num = m_IdCountMap[id];
	if (num == 1)
	{
		m_IdHashMap.erase(tagName);
		m_IdCountMap.erase(id);
	}
	else --num;
}

void Ref::SetTag(const string& tagName)
{
	RemoveId(m_strTag);
	m_strTag = tagName;

	if (tagName.size() && m_IdHashMap.find(tagName) == m_IdHashMap.end())
	{
		auto exist = m_IdHashMap.find(tagName);
		if (exist == m_IdHashMap.end())
		{
			m_id = PickUniqueId(tagName);
		}
		else {
			m_id = exist->second;
			++m_IdCountMap[m_id];
		}
	}
}