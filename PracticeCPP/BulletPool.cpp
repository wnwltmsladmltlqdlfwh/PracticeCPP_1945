#include "pch.h"
#include "BulletPool.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Actor.h"
#include "Bullet.h"

void BulletPool::Init(int poolSize)
{
    _pool.reserve(poolSize);

    for (int i = 0; i < poolSize; i++)
    {
        Bullet* bullet = new Bullet(nullptr, 0);
        bullet->SetActive(false);  // 비활성화 상태로 생성
        _pool.push_back(bullet);
    }
}

void BulletPool::Clear()
{
    for (Bullet* bullet : _pool)
        SAFE_DELETE(bullet);
    _pool.clear();

    for (Bullet* bullet : _activeList)
        SAFE_DELETE(bullet);
    _activeList.clear();
}

Bullet* BulletPool::Acquire(Actor* owner, int damage)
{
    Bullet* bullet = nullptr;

    if (!_pool.empty())
    {
        bullet = _pool.back();
        _pool.pop_back();
    }
    else
    {
        // 풀이 비었으면 새로 생성
        bullet = new Bullet(nullptr, 0);
    }

    bullet->Reset(owner, damage);  // 초기화
    bullet->SetActive(true);
    _activeList.push_back(bullet);

    return bullet;
}

void BulletPool::Release(Bullet* bullet)
{
    if (bullet == nullptr)
        return;

    bullet->SetActive(false);

    // activeList에서 제거
    auto it = std::find(_activeList.begin(), _activeList.end(), bullet);
    if (it != _activeList.end())
        _activeList.erase(it);

    _pool.push_back(bullet);
}