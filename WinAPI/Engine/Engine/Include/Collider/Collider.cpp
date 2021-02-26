#include <cassert>
#include "Collider.h"
#include "../Application/Window.h"
#include "../Object/Object.h"
#include "../Math.h"

Collider::Collider()
	:
	m_eCollType(CT_RECT),
	m_pObject(nullptr),
	m_bUIColl(false),
	m_tHitPoint()
{
}

Collider::Collider(const Collider& coll)
{
	*this = coll;
}

Collider::~Collider()
{
	// �� �浹ü�� �Ǻ��Ͽ� ����Ʈ�� ���� �༮���� ���ִ޶�� ��û�ؾ���
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
	// ���� ���� x ��ǥ�� �簢���� ���� �����ȿ� �ִ���
	// ���� ���� y ��ǥ�� �簢���� ���� �����ȿ� �ִ��� üũ�Ѵ�.
	// ���� �ϳ��� �´ٸ� �簢���� ��, �Ʒ�, ��, �� ���� �ȿ� �����Ѵٴ� ���̴�.
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

	// �簢���� 4���� ���� �ϳ��� �� �ȿ� ���� ��� �浹�̴�.
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

bool Collider::CollisionSphereToPixel(const Sphere& src, const vector<Pixel>& vecPixel, int iWidth, int iHeight)
{
	m_tHitPoint.x = -1;
	m_tHitPoint.y = -1;

	int iStartX = int(src.center.x - src.radius), iEndX = int(src.center.x + src.radius);
	int iStartY = int(src.center.y - src.radius), iEndY = int(src.center.y + src.radius);

	iStartX = iStartX < 0 ? 0 : iStartX;
	iEndX = iEndX >= iWidth ? iWidth - 1 : iEndX;

	iStartY = iStartY < 0 ? 0 : iStartY;
	iEndY = iEndY >= iHeight ? iHeight - 1 : iEndY;

	for (int i = iStartY; i <= iEndY; ++i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			if (Math::Distance(Pos(float(i), float(j)), src.center) > src.radius) continue;

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

bool Collider::CollisionRectToPoint(const Rect& src, const Pos& dst)
{
	// _cprintf("mouse pose : %2.1f %2.1f\n", dst.x, dst.y);
	if (dst.x < src.left)  return false;
	if (dst.x > src.right) return false;
	if (dst.y < src.top)   return false;
	if (dst.y > src.bottom) return false;

	return true;
}

bool Collider::CollisionSphereToPoint(const Sphere& src, const Pos& dst)
{
	return Math::Distance(src.center, dst) <= src.radius;
}

bool Collider::CollisionPixelToPoint(const vector<Pixel>& vecPixel, int iWidth, int iHeight, const Pos& dst)
{
	if (dst.x < 0 || dst.x >= iWidth || dst.y < 0 || dst.y >= iHeight)
		return false;

	int idx = int(dst.y) * iWidth + int(dst.x);
	const Pixel& pixel = vecPixel[idx];
	if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
	{
		m_tHitPoint = dst;
		return true;
	}

	return false;
}
