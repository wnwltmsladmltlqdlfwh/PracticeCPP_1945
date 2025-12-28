#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "Actor.h"

void CollisionManager::Init()
{

}

void CollisionManager::Update()
{
	//vector<Collider*>& colliders = _colliders;

    int totalColliders = 0;
    for (int i = 0; i < LAYER_MAXCOUNT; i++)
        totalColliders += _collidersByLayer[i].size();

    int bulletColliders = _collidersByLayer[LAYER_BULLET].size();

    OutputDebugStringA(std::format("Colliders: {}, BulletColliders: {}\n", totalColliders, bulletColliders).c_str());

	/*for (int32 i = 0; i < colliders.size(); i++)
	{
		for (int32 j = i + 1; j < colliders.size(); j++)
		{
			Collider* src = colliders[i];
			Collider* dest = colliders[j];

			if (src->GetOwner() == nullptr || dest->GetOwner() == nullptr)
				continue;

			LAYER_TYPE layerA = src->GetOwner()->GetLayer();
			LAYER_TYPE layerB = dest->GetOwner()->GetLayer();

			if (!_collisionMatrix[layerA][layerB])
				continue;

			if (src->CheckCollision(dest))
			{
				if (src->_collisionMap.contains(dest) == false)
				{
					src->GetOwner()->OnComponentBeginOverlap(src, dest);
					dest->GetOwner()->OnComponentBeginOverlap(dest, src);
					src->_collisionMap.insert(dest);
					dest->_collisionMap.insert(src);
				}
			}
			else
			{
				if (src->_collisionMap.contains(dest))
				{
					src->GetOwner()->OnComponentEndOverlap(src, dest);
					dest->GetOwner()->OnComponentEndOverlap(dest, src);
					src->_collisionMap.erase(dest);
					dest->_collisionMap.erase(src);
				}
			}
		}
	}*/
    for (int i = 0; i < LAYER_MAXCOUNT; i++)
    {
        for (int j = i; j < LAYER_MAXCOUNT; j++)
        {
            if (_collisionMatrix[i][j])
            {
                CheckCollisionBetweenLayers((LAYER_TYPE)i, (LAYER_TYPE)j);
            }
        }
    }
}

void CollisionManager::AddCollider(Collider* collider)
{
	//_colliders.push_back(collider);

    if (collider == nullptr || collider->GetOwner() == nullptr)
        return;

    LAYER_TYPE layer = collider->GetOwner()->GetLayer();
    _collidersByLayer[layer].push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider)
{
	//if (collider == nullptr)
	//	return;

	//// �ٸ� Collider���� _collisionMap���� ����
	//for (Collider* other : collider->_collisionMap)
	//{
	//	if (other)
	//		other->_collisionMap.erase(collider);
	//}
	//collider->_collisionMap.clear();

	//auto it = std::remove(_colliders.begin(), _colliders.end(), collider);
	//_colliders.erase(it, _colliders.end());

    if (collider == nullptr)
        return;

    for (Collider* other : collider->_collisionMap)
    {
        if (other)
            other->_collisionMap.erase(collider);
    }
    collider->_collisionMap.clear();

    if (collider->GetOwner())
    {
        LAYER_TYPE layer = collider->GetOwner()->GetLayer();
        auto& vec = _collidersByLayer[layer];
        auto it = std::remove(vec.begin(), vec.end(), collider);
        vec.erase(it, vec.end());
    }
}

void CollisionManager::CheckCollisionBetweenLayers(LAYER_TYPE layerA, LAYER_TYPE layerB)
{
    vector<Collider*>& collidersA = _collidersByLayer[layerA];
    vector<Collider*>& collidersB = _collidersByLayer[layerB];

    if (layerA == layerB)
    {
        for (int32 i = 0; i < collidersA.size(); i++)
        {
            for (int32 j = i + 1; j < collidersA.size(); j++)
            {
                Collider* src = collidersA[i];
                Collider* dest = collidersA[j];

                if (src->GetOwner() == nullptr || dest->GetOwner() == nullptr)
                    continue;

                if (src->CheckCollision(dest))
                {
                    if (!src->_collisionMap.contains(dest))
                    {
                        src->GetOwner()->OnComponentBeginOverlap(src, dest);
                        dest->GetOwner()->OnComponentBeginOverlap(dest, src);
                        src->_collisionMap.insert(dest);
                        dest->_collisionMap.insert(src);
                    }
                }
                else
                {
                    if (src->_collisionMap.contains(dest))
                    {
                        src->GetOwner()->OnComponentEndOverlap(src, dest);
                        dest->GetOwner()->OnComponentEndOverlap(dest, src);
                        src->_collisionMap.erase(dest);
                        dest->_collisionMap.erase(src);
                    }
                }
            }
        }
    }
    else
    {
        // 다른 레이어간 체크
        for (Collider* src : collidersA)
        {
            for (Collider* dest : collidersB)
            {
                if (src->GetOwner() == nullptr || dest->GetOwner() == nullptr)
                    continue;

                if (src->CheckCollision(dest))
                {
                    if (!src->_collisionMap.contains(dest))
                    {
                        src->GetOwner()->OnComponentBeginOverlap(src, dest);
                        dest->GetOwner()->OnComponentBeginOverlap(dest, src);
                        src->_collisionMap.insert(dest);
                        dest->_collisionMap.insert(src);
                    }
                }
                else
                {
                    if (src->_collisionMap.contains(dest))
                    {
                        src->GetOwner()->OnComponentEndOverlap(src, dest);
                        dest->GetOwner()->OnComponentEndOverlap(dest, src);
                        src->_collisionMap.erase(dest);
                        dest->_collisionMap.erase(src);
                    }
                }
            }
        }
    }
}
