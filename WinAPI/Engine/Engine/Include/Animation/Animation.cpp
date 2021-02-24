#include "Animation.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourceManager.h"
#include "../Object/Object.h"

Animation::Animation()
	:
	m_pCurClip(nullptr),
	m_pObj(nullptr)
{
}

Animation::Animation(const Animation& anim)
{
	*this = anim;

	m_mapClip.clear();

	unordered_map<string, AnimationClip*>::const_iterator iter;
	unordered_map<string, AnimationClip*>::const_iterator iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		AnimationClip* pClip = new AnimationClip;

		*pClip = *iter->second;

		for (size_t i = 0; i < pClip->vecTexture.size(); ++i)
		{
			pClip->vecTexture[i]->AddRef();
		}
	}

	m_pCurClip = nullptr;

	m_strCurClip = "";
	SetCurrentClip(anim.m_strCurClip);
}

Animation::~Animation()
{
	unordered_map<string, AnimationClip*>::iterator iter;
	unordered_map<string, AnimationClip*>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecTexture[i]);
		}
		SAFE_DELETE(iter->second)
	}

	m_mapClip.clear();
}

bool Animation::AddClip(const string& strName,
						ANIMATION_TYPE eType,
						ANIMATION_OPTION eOption,
						float fAnimationTime,
						float fAnimationLimitTime,
						int iFrameMaxX,
						int iFrameMaxY,
						int iStartX,
						int iStartY,
						int iLengthX,
						int iLengthY,
						float fOptionLimitTime,
						const string& strTexKey,
						const wchar_t* pFileName,
						const string& strPathKey = TEXTURE_PATH)
{
	AnimationClip* pClip = new AnimationClip;
	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->iStartX = iStartX;
	pClip->iStartY = iStartY;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / (iLengthX * iLengthY);

	Texture* pTex = RESOURCE_MANAGER.LoadTexture(strTexKey, pFileName, strPathKey);
	pClip->vecTexture.push_back(pTex);
	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = iStartX;
	pClip->iFrameY = iStartY;
	pClip->fOptionTime = 0.f;

	m_mapClip.insert(make_pair(strName, pClip));

	if(m_strDefaultClip.empty())
		SetDefaultClip(strName);

	if(m_strCurClip.empty())
		SetCurrentClip(strName);
	
	return true;
}

void Animation::SetClipColorKey(const string& strClip, unsigned char r, unsigned char g, unsigned char b)
{
	AnimationClip* pClip = FindClip(strClip);

	if (!pClip)
		return;

	for (size_t i = 0; i < pClip->vecTexture.size(); ++i)
	{
		pClip->vecTexture[i]->SetColorKey(r, g, b);
	}
}


void Animation::SetCurrentClip(const string& strCurClip)
{
	ChangeClip(strCurClip);
	m_strCurClip = strCurClip;
}

void Animation::SetDefaultClip(const string& strDefaultClip)
{
	m_strDefaultClip = strDefaultClip;
}

void Animation::ChangeClip(const string& strClip)
{
	if (m_strCurClip == strClip)
		return;

	m_strCurClip = strClip;

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = m_pCurClip->iStartX;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}

	m_pCurClip = FindClip(strClip);

	if (m_pCurClip->eType == AT_ATLAS)
		m_pObj->SetTexture(m_pCurClip->vecTexture[0]);

	else if (m_pCurClip->eType == AT_FRAME)
		m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrameX]);
}

AnimationClip* Animation::FindClip(const string& strName)
{
	unordered_map<string, AnimationClip*>::iterator iter = m_mapClip.find(strName);
	if (iter == m_mapClip.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool Animation::Init()
{
	return false;
}

void Animation::Update(float fTime)
{
	m_pCurClip->fAnimationTime += fTime;

	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationFrameTime -= m_pCurClip->fAnimationFrameTime;

		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			m_pCurClip->iFrameX = m_pCurClip->iStartX;

			++m_pCurClip->iFrameY;

			if (m_pCurClip->iFrameY - m_pCurClip->iStartY == m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;
				switch (m_pCurClip->eOption)
				{
				case AO_ONCE_RETURN:
					ChangeClip(m_strDefaultClip);
					break;
				case AO_ONCE_DESTROY:
					m_pObj->Die();
					break;
				case AO_TIME_RETURN:
					break;
				case AO_TIME_DESTROY:
					break;
				}
			}
		}
	}
}

Animation* Animation::Clone()
{
	return new Animation(*this);
}