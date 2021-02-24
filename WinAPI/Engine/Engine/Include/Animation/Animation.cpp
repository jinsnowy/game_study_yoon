#include "Animation.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourceManager.h";

Animation::Animation()
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
						const string& strPathKey)
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
	
	Texture* pTex = RESOURCE_MANAGER.LoadTexture(strTexKey, pFileName, strPathKey);

	pClip->vecTexture.push_back(pTex);

	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = 0;
	pClip->iFrameY = 0;
	pClip->fOptionTime = 0.f;

	m_mapClip.insert(make_pair(strName, pClip));

	return false;
}

bool Animation::Init()
{
	return false;
}

void Animation::Update(float fTime)
{
}

Animation* Animation::Clone()
{
	return new Animation(*this);
}
