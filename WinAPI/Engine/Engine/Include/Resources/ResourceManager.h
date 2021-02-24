#include "../framework.h"

class ResourceManager {
	DECLARE_SINGLE(ResourceManager)
private:
	unordered_map<string, class Texture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC	      m_hDC;
	class Texture* m_pBackBuffer;
public:
	class Texture* GetBackBuffer() const;
public:
	bool Init(HINSTANCE hInst, HDC hDC);
	// strkey = 텍스쳐 명칭, pFileName = 파일 이름, strPathKey = 텍스쳐 base 경로 
	class Texture* LoadTexture(const string& strKey,
								const wchar_t* pFileName,
								const string& strPathKey = TEXTURE_PATH);
	class Texture* FindTexture(const string& strKey);
};