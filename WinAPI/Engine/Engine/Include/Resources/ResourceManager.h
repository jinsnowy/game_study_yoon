#include "../framework.h"

class ResourceManager {
	DECLARE_SINGLE(ResourceManager)
private:
	unordered_map<string, class Texture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC	      m_hDC;
	class Texture* m_pBackBuffer;
private:
	class Texture* m_pEmptyBuffer;
	BLENDFUNCTION m_tBlenFunc;
public:
	class Texture* GetBackBuffer() const;
	class Texture* GetEmptyBuffer() const;
	void SetAlphaChannel(int alpha) { m_tBlenFunc.SourceConstantAlpha = alpha; }
	BLENDFUNCTION GetBlendFunc() const { return m_tBlenFunc; }
public:
	bool Init(HINSTANCE hInst, HDC hDC);
	// strkey = 텍스쳐 명칭, pFileName = 파일 이름, strPathKey = 텍스쳐 base 경로 
	class Texture* LoadTexture(const string& strKey,
								const wchar_t* pFileName,
								const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(FILE* pFile);
	class Texture* FindTexture(const string& strKey);
	void AddTexture(const string& strKey, Texture* pTexture);
};