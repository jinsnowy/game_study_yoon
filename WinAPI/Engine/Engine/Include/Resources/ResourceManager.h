#include "../framework.h"

class ResourceManager {
	DECLARE_SINGLE(ResourceManager)

private:
	unordered_map<string, class Texture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC	      m_hDC;
public:
	bool Init(HINSTANCE hInst, HDC hDC);
	class Texture* LoadTexture(const string& strKey, const char* pFileName, const string& strPathKey);
	class Texture* FindTexture(const string& strKey);
};