#include "CInGameScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMinion.h"
#include "CLayer.h"

CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init()
{
	// �θ� Scene Ŭ������ �ʱ�ȭ �Լ��� ȣ�����ش�.
	if (!CScene::Init())
	{
		return false;
	}

	CLayer* pLayer = FindLayer("Default");

	CPlayer* pPlayer = CObject::CreateObj<CPlayer>("Player", pLayer);
	SAFE_RELEASE(pPlayer);
	CMinion* pMinion = CObject::CreateObj<CMinion>("Minion", pLayer);
	SAFE_RELEASE(pMinion);

	return true;
}
