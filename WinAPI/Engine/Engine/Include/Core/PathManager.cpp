#include "PathManager.h"

DEFINITION_SINGLE(PathManager)

PathManager::PathManager()
{
}

PathManager::~PathManager()
{
}

bool PathManager::Init()
{
	char strPath[MAX_PATH] = {};

	// ���� ���丮 ���� ���� ������ �ִ� ������ ���Ѵ�.
	GetModuleFileName(NULL, strPath, MAX_PATH);

	// ��Ʈ ��� ã��
	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			memset(strPath + (i + 1), 0, sizeof(char) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	// Texture ��� ����
	if (!CreatePath(TEXTURE_PATH, "Texture\\"))
		return false;

	return true;
}

bool PathManager::CreatePath(const string& strKey, const char* pPath, const string& strBaseKey)
{
	const char* pBasePath = FindPath(strBaseKey);

	string strPath;
	if (pBasePath)
		strPath = pBasePath;

	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const char* PathManager::FindPath(const string& strKey)
{
	auto iter = m_mapPath.find(strKey);
	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second.c_str();
}
