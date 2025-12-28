#pragma once
#include "FlipbookActor.h"

class Collider;

class PowerUpItem : public FlipbookActor
{
    using Super = FlipbookActor;
public:
    PowerUpItem();
    virtual ~PowerUpItem() override;

    virtual void BeginPlay() override;
    virtual void Tick() override;
    virtual void Render(HDC hdc) override;

    virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;

private:
    float _speed = 150.f;
    float _direction = 1.f;
    bool _bouncing = false;
    float _bounceMinY = 100.f;
    float _bounceMaxY = 0.f;
};

