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
	Safe_Release_Map(m_mapTexture);
}

bool ResourceManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;
	return true;
}

Texture* ResourceManager::LoadTexture(const string& strKey,
									  const char* pFileName,
									  const string& strPathKey)
{
	Texture* pTexture = FindTexture(strKey);
	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return nullptr;
	}

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
