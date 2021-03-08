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

	// ����۸� �ҷ��´�.
	m_pBackBuffer = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	// �����
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
	// ���� ������ ��ϵ� �ؽ������� Ȯ���Ѵ�.
	Texture* pTexture = FindTexture(strKey);
	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return nullptr;
	}

	// ���� �ؽ��ĸ� ��� 
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

	// key�� ���̸� �����Ѵ�.
	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;

	// FileName ����
	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);
	strFileName[iLength] = 0;

	// PathKey ����
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

