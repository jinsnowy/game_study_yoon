#include "../framework.h"

class ResourceManager {
	DECLARE_SINGLE(ResourceManager)
private:
	unordered_map<string, class Texture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC	      m_hDC;
private:
	class Texture* m_pBackBuffer = nullptr;
	class Texture* m_pTempBuffer = nullptr;
	class Texture* m_pEmptyBuffer = nullptr;
	BLENDFUNCTION m_tBlenFunc = {};
	BLENDFUNCTION m_tTransparent = {};
public:
	class Texture* GetBackBuffer() const;
	class Texture* GetEmptyBuffer() const;
	class Texture* GetTempBuffer() const;
	void SetAlphaChannel(int alpha) { m_tBlenFunc.SourceConstantAlpha = alpha; }
	BLENDFUNCTION GetBlendFunc() const { return m_tBlenFunc; }
	const BLENDFUNCTION& GetTransparentFunc() const { return m_tTransparent; }
	void ClearBuffer(Texture* pTex, int px, int py, int w, int h);
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