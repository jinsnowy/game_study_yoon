#include "ResourceManager.h"
#include "Texture.h"
#include "../Object/Object.h"
#include "../Application/Window.h"

DEFINITION_SINGLE(ResourceManager)

ResourceManager::ResourceManager()
	: m_hInst(nullptr),
	  m_hDC(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_RELEASE(m_pEmptyBuffer);
	Safe_Release_Map(m_mapTexture);
}

Texture* ResourceManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

Texture* ResourceManager::GetEmptyBuffer() const
{
	m_pEmptyBuffer->AddRef();
	return m_pEmptyBuffer;
}

bool ResourceManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	// 백버퍼를 불러온다.
	m_pBackBuffer = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	// 빈버퍼
	m_pEmptyBuffer = Texture::CreateEmptyTexture(hDC, GETRESOLUTION.x, GETRESOLUTION.y);

	m_tBlenFunc.BlendOp = AC_SRC_OVER;
	m_tBlenFunc.BlendFlags = 0;
	m_tBlenFunc.SourceConstantAlpha = 0;
	m_tBlenFunc.AlphaFormat = 0;

	m_tTransparent.BlendOp = AC_SRC_OVER;
	m_tTransparent.BlendFlags = 0;
	m_tTransparent.SourceConstantAlpha = 0;
	m_tTransparent.AlphaFormat = 100;
	return true;
}

Texture* ResourceManager::LoadTexture(const string& strKey,
									  const wchar_t* pFileName,
									  const string& strPathKey)
{
	assert(strKey.size());
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

Texture* ResourceManager::LoadTexture(FILE* pFile)
{
	int iLength = 0;

	char strKey[MAX_PATH] = {};
	char strPathKey[MAX_PATH] = {};
	wchar_t strFileName[MAX_PATH] = {};

	// key의 길이를 저장한다.
	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;

	// FileName 저장
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);
	strFileName[iLength] = 0;

	// PathKey 저장
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strPathKey, 1, iLength, pFile);
	strPathKey[iLength] = 0;

	Texture* pTexture = RESOURCE_MANAGER->LoadTexture(strKey, strFileName, strPathKey);

	// ColorKey
	if (pTexture)
	{
		fread(&pTexture->m_bColorKeyEnable, 1, 1, pFile);
		fread(&pTexture->m_ColorKey, sizeof(COLORREF), 1, pFile);
	}

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

void ResourceManager::AddTexture(const string& strKey, Texture* pTexture)
{
	if (FindTexture(strKey))
		return;
	
	if (pTexture)
	{
		pTexture->AddRef();
		m_mapTexture.insert(make_pair(strKey, pTexture));
	}
}

