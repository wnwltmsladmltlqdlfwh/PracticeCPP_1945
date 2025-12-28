#include "pch.h"
#include "PowerUpItem.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "DevScene.h"
#include "Player.h"
#include "Collider.h"
#include "BoxCollider.h"

PowerUpItem::PowerUpItem()
{
    SetLayer(LAYER_ITEM);
    _bounceMaxY = GWinSizeY - 50.f;
}

PowerUpItem::~PowerUpItem()
{
    for (Component* component : _components)
    {
        if (Collider* collider = dynamic_cast<Collider*>(component))
            GET_SINGLE(CollisionManager)->RemoveCollider(collider);
    }
}

void PowerUpItem::BeginPlay()
{
    Super::BeginPlay();
}

void PowerUpItem::Tick()
{
    Super::Tick();

    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    _pos.y += _direction * _speed * deltaTime;

    if (!_bouncing && _pos.y >= _bounceMaxY)
    {
        _bouncing = true;
        _direction = -1.f;
    }

    if (_bouncing)
    {
        if (_pos.y <= _bounceMinY)
            _direction = 1.f;
        else if (_pos.y >= _bounceMaxY)
            _direction = -1.f;
    }
}

void PowerUpItem::Render(HDC hdc)
{
    Super::Render(hdc);
}

void PowerUpItem::OnComponentBeginOverlap(Collider* collider, Collider* other)
{
    Actor* otherActor = other->GetOwner();
    if (otherActor == nullptr)
        return;

    // 플레이어와 충돌했을 때
    if (Player* player = dynamic_cast<Player*>(otherActor))
    {
        player->AddBulletCount();

        // 아이템 삭제
        if (DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene()))
        {
            scene->RemoveActor(this);
        }
    }
}