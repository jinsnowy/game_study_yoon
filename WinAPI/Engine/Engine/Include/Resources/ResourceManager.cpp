#include "ResourceManager.h"
#include "Texture.h"

DEFINITION_SINGLE(ResourceManager)

ResourceManager::ResourceManager()
	: m_hInst(nullptr),
	  m_hDC(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	SAFE_RELEASE(m_pBackBuffer);
	Safe_Release_Map(m_mapTexture);
}

Texture* ResourceManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

bool ResourceManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	// 백버퍼를 불러온다.
	m_pBackBuffer = LoadTexture("BackBuffer", "BackBuffer.bmp");

	return true;
}

Texture* ResourceManager::LoadTexture(const string& strKey,
									  const char* pFileName,
									  const string& strPathKey)
{
	// 먼저 그전에 등록된 텍스쳐인지 확인한다.
	Texture* pTexture = FindTexture(strKey);
	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return nullptr;
	}

	// 새로 텍스쳐를 등록 
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourceManager::FindTexture(const string& strKey)
{
	auto iter = m_mapTexture.find(strKey);
	if (iter == m_mapTexture.end())
		return nullptr;

	iter->second->AddRef();
	return iter->second;
}