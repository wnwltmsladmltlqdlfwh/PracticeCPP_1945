#include "pch.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Actor.h"

Collider::Collider(ColliderType colliderType) : _colliderType(colliderType)
{

}

Collider::~Collider()
{
	// ���� �����ϴ� �ٸ� Collider���� _collisionMap���� ���� ����
	for (Collider* other : _collisionMap)
	{
		if (other)
			other->_collisionMap.erase(this);
	}
	_collisionMap.clear();
}

void Collider::BeginPlay()
{

}

void Collider::TickComponent()
{

}

void Collider::Render(HDC hdc)
{

}

bool Collider::CheckCollision(Collider* other)
{
	return false;
}

bool Collider::CheckCollisionBox2Box(BoxCollider* b1, BoxCollider* b2)
{
	Vec2 p1 = b1->GetOwner()->GetPos();
	Vec2 s1 = b1->GetSize();

	Vec2 p2 = b2->GetOwner()->GetPos();
	Vec2 s2 = b2->GetSize();

	float minX_1 = p1.x - s1.x / 2;
	float maxX_1 = p1.x + s1.x / 2;
	float minY_1 = p1.y - s1.y / 2;
	float maxY_1 = p1.y + s1.y / 2;

	float minX_2 = p2.x - s2.x / 2;
	float maxX_2 = p2.x + s2.x / 2;
	float minY_2 = p2.y - s2.y / 2;
	float maxY_2 = p2.y + s2.y / 2;

	if (maxX_2 < minX_1)
		return false;

	if (maxX_1 < minX_2)
		return false;

	if (maxY_1 < minY_2)
		return false;

	if (maxY_2 < minY_1)
		return false;

	return true;
}

bool Collider::CheckCollisionSphere2Box(SphereCollider* s1, BoxCollider* b2)
{
	Vec2 circlePos = s1->GetOwner()->GetPos();
	float radius = s1->GetRadius();

	Vec2 boxPos = b2->GetOwner()->GetPos();
	Vec2 boxSize = b2->GetSize();

	// Box 경계
	float boxLeft = boxPos.x - boxSize.x / 2.f;
	float boxRight = boxPos.x + boxSize.x / 2.f;
	float boxTop = boxPos.y - boxSize.y / 2.f;
	float boxBottom = boxPos.y + boxSize.y / 2.f;

	// 원의 중심에서 Box까지 가장 가까운 점 찾기
	float closestX = max(boxLeft, min(circlePos.x, boxRight));
	float closestY = max(boxTop, min(circlePos.y, boxBottom));

	// 가장 가까운 점과 원 중심 사이의 거리
	float distX = circlePos.x - closestX;
	float distY = circlePos.y - closestY;
	float distSquared = distX * distX + distY * distY;

	return distSquared <= radius * radius;
}

bool Collider::CheckCollisionSphere2Sphere(SphereCollider* s1, SphereCollider* s2)
{
	Vec2 p1 = s1->GetOwner()->GetPos();
	float r1 = s1->GetRadius();

	Vec2 p2 = s2->GetOwner()->GetPos();
	float r2 = s2->GetRadius();

	Vec2 dir = p1 - p2;
	float dist = dir.Length();

	return dist <= r1 + r2;
}