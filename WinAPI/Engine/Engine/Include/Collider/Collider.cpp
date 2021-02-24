#include "Collider.h"
#include "../Object/Object.h"
#include "../Math.h"

Collider::Collider()
	:
	m_eCollType(CT_RECT),
	m_pObject(nullptr)
{
}

Collider::Collider(const Collider& coll)
{
	*this = coll;
}

Collider::~Collider()
{
	// 날 충돌체로 판별하여 리스트로 가진 녀석들을 없애달라고 요청해야함
	list<Collider*>::iterator iter;
	list<Collider*>::iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollisionList(this);
	}
}

void Collider::SetObj(Object* pObject)
{
	m_pObject = pObject;
}

void Collider::Input(float dt)
{
}

int Collider::Update(float dt)
{
	return 0;
}

int Collider::LateUpdate(float dt)
{ 
	return 0;
}

bool Collider::CheckCollision(Collider* pDst)
{
	return true;
}


void Collider::Draw(HDC hdc, float dt)
{
}

bool Collider::CollisionRectToRect(const Rect& src, const Rect& dst)
{
	if (src.right < dst.left) return false;
	if (src.left > dst.right) return false;
	if (src.bottom < dst.top) return false;
	if (src.top > dst.bottom) return false;
	return true;
}

bool Collider::CollisionRectToSphere(const Rect& src, const Sphere& dst)
{
	// 원의 중점 x 좌표가 사각형의 가로 영역안에 있는지
	// 원의 중점 y 좌표가 사각형의 세로 영역안에 있는지 체크한다.
	// 둘중 하나라도 맞다면 사각형의 위, 아래, 좌, 우 영역 안에 존재한다는 뜻이다.
	if ((src.left <= dst.center.x && dst.center.x <= src.right)
		|| (src.top <= dst.center.y && dst.center.y <= src.bottom))
	{
		Rect tRC = src;
		tRC.left -= dst.radius;
		tRC.right += dst.radius;
		tRC.top -= dst.radius;
		tRC.bottom += dst.radius;
	
		if (dst.center.x < tRC.left)
			return false;
		if (dst.center.x > tRC.right)
			return false;
		if (dst.center.y < tRC.top)
			return false;
		if (dst.center.y > tRC.bottom)
			return false;
		return true;
	}

	// 사각형의 4개의 점이 하나라도 원 안에 있을 경우 충돌이다.
	Pos tPos[4] = {};
	tPos[0] = Pos(src.left, src.top);
	tPos[1] = Pos(src.right, src.top);
	tPos[2] = Pos(src.left, src.bottom);
	tPos[3] = Pos(src.right, src.bottom);

	for (int i = 0; i < 4; i++)
	{
		if (Math::Distance(tPos[i], dst.center) <= dst.radius)
			return true;
	}
	return false;
}

bool Collider::CollisionSphereToSphere(const Sphere& src, const Sphere& dst)
{
	const auto dist = Math::Distance(src.center, dst.center);
	return dist <= src.radius + dst.radius;
}

bool Collider::CollisionRectToPixel(const Rect& src, const vector<Pixel>& vecPixel, int iWidth, int iHeight)
{
	m_tHitPoint.x = -1;
	m_tHitPoint.y = -1;

	int iStartX, iEndX;
	int iStartY, iEndY;

	iStartX = int(src.left) < 0 ? 0 : int(src.left);
	iEndX = int(src.right) >= iWidth ? iWidth - 1 : int(src.right);

	iStartY =  int(src.top) < 0 ? 0 : int(src.top);
	iEndY = int(src.bottom) >= iHeight ? iHeight - 1 : int(src.bottom);

	for (int i = iStartY; i <= iEndY; ++i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = i * iWidth + j;
			const Pixel& pixel = vecPixel[idx];
			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
			{
				m_tHitPoint.x = float(j);
				m_tHitPoint.y = float(i);
				return true;
			}
		}
	}

	return false;
}
