#pragma once
#include "../Object/Ref.h"
#include "../framework.h"
class Animation : public Ref
{
private:
	friend class Object;
private:
	Animation();
	Animation(const Animation& anim);
	~Animation();
private:
	unordered_map<string, AnimationClip*> m_mapClip;
public:
	bool AddClip(const string& strName,
				ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
				float	fAnimationTime,
				float   fAnimationLimitTime,
				int		iFrameMaxX,
				int		iFrameMaxY,
				int		iStartX,
				int		iStartY,
				int		iLengthX,
				int		iLengthY,
				float	fOptionLimitTime,
				const string& strTexKey,
				const wchar_t* pFileName,
				const string& strPathKey);
public:
	bool Init();
	void Update(float fTime);
	Animation* Clone();
};
