#pragma once
#include "FlipbookActor.h"

class Flipbook;
class Collider;

class Player : public FlipbookActor
{
	using Super = FlipbookActor;
public:
	Player();
	virtual ~Player() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other) override;

	void SetDamage(int setDmg) { _bulletDamage = setDmg; }
	int GetDamage() { return _bulletDamage; }

	void AddBulletCount()
	{
		if (_bulletCount < 3)
			_bulletCount++;
	}
	int GetBulletCount() { return _bulletCount; }

private:
	Flipbook*	_flipbookIdle = nullptr;
	Flipbook*	_flipbookLeft = nullptr;
	Flipbook*	_flipbookRight = nullptr;

private:
	float _delayBullet = 0.f;
	int _bulletDamage = 0;
	int _bulletCount = 1;
};

