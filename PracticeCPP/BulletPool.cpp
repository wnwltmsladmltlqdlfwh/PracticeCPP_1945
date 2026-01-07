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
        bullet->SetActive(false);
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
        bullet = new Bullet(nullptr, 0);
    }

    bullet->Reset(owner, damage);
    bullet->SetActive(true);
    _activeList.push_back(bullet);

    return bullet;
}

void BulletPool::Release(Bullet* bullet)
{
    if (bullet == nullptr)
        return;

    bullet->SetActive(false);

    auto it = std::find(_activeList.begin(), _activeList.end(), bullet);
    if (it != _activeList.end())
        _activeList.erase(it);

    _pool.push_back(bullet);
}

void BulletPool::TickAll()
{
    vector<Bullet*> copyList = _activeList;
    for (Bullet* bullet : copyList)
    {
        if (bullet && bullet->IsActive())
            bullet->Tick();
    }
}

void BulletPool::RenderAll(HDC hdc)
{
    for (Bullet* bullet : _activeList)
    {
        if (bullet && bullet->IsActive())
            bullet->Render(hdc);
    }
}