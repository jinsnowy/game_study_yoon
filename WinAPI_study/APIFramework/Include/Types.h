#pragma once

typedef struct _tagResolution
{
	unsigned int iW;
	unsigned int iH;
}RESOLUTION, *PRESOLUTION;


typedef struct _tagPosition
{
	float x, y;

	_tagPosition() :
		x(0.f),
		y(0.f)
	{
	}
	_tagPosition(float _x, float _y)
		:
		x(_x),
		y(_y)
	{

	}
	_tagPosition(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}
	_tagPosition(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}
	void operator=(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}
	void operator+=(const _tagPosition& pos)
	{
		x += pos.x;
		y += pos.y;
	}
	_tagPosition operator*(float f) const
	{
		_tagPosition pos;
		pos.x = x * f;
		pos.y = y * f;
		return pos;
	}
	_tagPosition operator/(float f) const
	{
		_tagPosition pos;
		pos.x = x / f;
		pos.y = y / f;
		return pos;
	}
}POSITION, *PPOSITION, _SIZE, *_PSIZE;