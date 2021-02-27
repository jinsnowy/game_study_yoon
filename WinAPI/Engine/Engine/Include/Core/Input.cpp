#include "Input.h"
#include "../Object/Mouse.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderPoint.h"
#include "../Collider/CollisionManager.h"

DEFINITION_SINGLE(Input)
Input::Input()
	: m_hWnd(nullptr),
	  m_pCreateKey(nullptr),
	  m_pMouse(nullptr),
	  m_tMouseMove(),
	  m_tMousePos()
{
}

Input::~Input()
{
	Object::EraseObject(m_pMouse);
	SAFE_RELEASE(m_pMouse);
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
	AddKey(VK_LBUTTON, "MouseLButton");
	AddKey(VK_RBUTTON, "MouseRButton");

	// ���콺 ��ġ�� ������ �Լ��̴�.
	GetCursorPos(&m_tMousePos);

	// ���콺 ����
	m_pMouse = Object::CreateObject<Mouse>("Mouse");

	m_pMouse->SetTexture("MouseTexture", L"mouse.bmp");
	m_pMouse->SetSize(32, 19);
	m_pMouse->SetColorKey(255, 255, 255);

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

	m_pMouse->Update(dt);
	m_pMouse->LateUpdate(dt);
	COLLISION_MANAGER->AddObject(m_pMouse);
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

Pos Input::GetMouseClientPos() const
{
	return m_pMouse->GetPos();
}

Pos Input::GetMouseWorldPos() const
{
	return m_pMouse->GetWorldPos();
}

Pos Input::GetMouseMove() const
{
	return m_pMouse->GetMove();
}

Input::KeyInfo* Input::FindKey(const string& strKey) const
{
	const auto iter = m_mapKey.find(strKey);
	if (iter == m_mapKey.end())
		return nullptr;
	return iter->second;
}