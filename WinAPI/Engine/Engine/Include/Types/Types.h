#pragma once
#include <vector>
#include "Vec2.h"

typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Pixel;

enum TILE_TYPE 
{
	TL_NONE = 0,
	TL_TREE,
};

enum OBJECT_TYPE
{
	OBJ_BUILDING,
	OBJ_TILE_INNER,
	OBJ_PLANT,
	OBJ_END
};

enum EDIT_MODE
{
	SEL_GROUND = 0,
	SEL_STATIC,
	SEL_TILEOBJECT,
	SEL_OBJECT,
	SEL_OPTION,
	SEL_END
};

enum STAGE_SHOW {
	SHOW_ALL,
	SHOW_GROUND,
	SHOW_OBJECT,
	SHOW_STATIC,
	SHOW_END
};

enum STAGE_TAG
{
	ST_GROUND = 0,
	ST_OBJECT,
	ST_STATIC,
	ST_END
};

enum SOUND_CHANNEL
{
	SD_BACKGROUND,
	SD_MAIN,
	SD_EFFECT,
	SD_END
};

// Scene Type
enum SCENE_CREATE
{
	SC_CURRENT,
	SC_NEXT,
	SC_END
};

// Sceene
enum SCENE_CHANGE
{
	SC_NONE,
	SC_CHANGE
};

// Tile Option
enum TILE_OPTION
{
	TO_NONE,
	TO_NOMOVE,
	TO_CROP_GROUND,
	TO_END
};

// Move dir type
enum MOVE_DIR
{
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT
};

// Collider Type
enum COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_LINE,
	CT_POINT,
	CT_PIXEL,
	CT_END
};

// Collision State
enum COLLISION_STATE
{
	CS_ENTER,
	CS_STAY,
	CS_LEAVE,
	CS_END
};

// Animation Type
enum ANIMATION_TYPE
{
	AT_ATLAS,
	AT_FRAME,
	AT_END
};

// Animation Option
enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_RETURN,  // 한 번 돌고 기본 모션으로 돌아감
	AO_ONCE_DESTROY, // 한 번 돌고 오브젝트 삭제 (이펙트)
	AO_TIME_RETURN,  // (버프 같은) 타임 오버시 기본 모션으로 돌아감 
	AO_TIME_DESTROY  // 타임 오버시 오브젝트 삭제
};

typedef struct _tagAnimationClip
{
	ANIMATION_TYPE	   eType;
	ANIMATION_OPTION   eOption;
	std::vector<class Texture*> vecTexture;
	float	fAnimationTime;
	float   fAnimationLimitTime;
	float	fAnimationFrameTime;
	int		iFrameX;
	int		iFrameY;
	int		iFrameMaxX;
	int		iFrameMaxY;
	int		iStartX;	
	int		iStartY;
	int		iLengthX;
	int		iLengthY;
	float	fOptionTime;
	float	fOptionLimitTime;
	Vec2F	tFrameSize;
}AnimationClip;
