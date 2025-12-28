#pragma once
#include "SpriteActor.h"

class Sprite;

class EnemyBase : public SpriteActor
{
	using Super = SpriteActor;
public:
	EnemyBase();
	virtual ~EnemyBase() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other) override;

private:
	Sprite* _sprite = nullptr;
	Stat _enemyStat;
};

