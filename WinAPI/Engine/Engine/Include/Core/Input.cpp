#include "Input.h"
DEFINITION_SINGLE(Input)

Input::Input()
	: m_hWnd(nullptr),
	  m_pCreateKey(nullptr)
{
}

Input::~Input()
{
	Safe_Delete_Map(m_mapKey);
}

bool Input::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddKey('W', "MoveFront");
	AddKey('S', "MoveBack");
	AddKey("MoveLeft", 'A');
	AddKey("MoveRight", 'D');
	AddKey("Fire", VK_SPACE);
	AddKey(VK_CONTROL, "Skill1", '1');

	return true;
}

void Input::Update(float dt)
{
	auto iterEnd = m_mapKey.end();
	for (auto iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
			{
				iPushCount++;
			}
		}

		if (iPushCount == iter->second->vecKey.size())
		{
			// �� �� �� ���Ⱦ��� ����
			if (!iter->second->bDown && !iter->second->bPress)
			{
				iter->second->bPress = true;
				iter->second->bDown = true;
			}
			// �� �� ���������� ����
			else if (iter->second->bDown)
			{
				iter->second->bDown = false;
			}
		}
		else {
			// Ű�� ����
			// ����������
			if (iter->second->bDown || iter->second->bPress)
			{
				iter->second->bDown = false;
				iter->second->bPress = false;
				iter->second->bUp = true;
			}
			// �� ���� ����
			else if (iter->second->bUp)
			{
				iter->second->bUp = false;
			}
		}
	}
}

bool Input::KeyDown(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bDown;
}

bool Input::KeyPress(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bPress;
}

bool Input::KeyUp(const string& strKey) const
{
	KeyInfo* pInfo = FindKey(strKey);
	if (!pInfo)
		return false;
	return pInfo->bUp;
}

Input::KeyInfo* Input::FindKey(const string& strKey) const
{
	const auto iter = m_mapKey.find(strKey);
	if (iter == m_mapKey.end())
		return nullptr;
	return iter->second;
}