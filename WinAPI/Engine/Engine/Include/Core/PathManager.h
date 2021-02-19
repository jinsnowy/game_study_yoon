#pragma once
#include "../framework.h"
class PathManager
{
	DECLARE_SINGLE(PathManager)
private:
	unordered_map<string, string> m_mapPath;
public:
	bool Init();
	bool CreatePath(const string& strKey, const char* pPath, const string& strBaseKey = ROOT_PATH);
	const char* FindPath(const string& strKey);
};

