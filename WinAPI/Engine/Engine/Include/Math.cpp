#include "Math.h"

float Math::Distance(const Pos& tPos1, const Pos& tPos2)
{
	float x = tPos1.x - tPos2.x;
	float y = tPos1.y - tPos2.y;

	return sqrtf(x*x+y*y);
}
