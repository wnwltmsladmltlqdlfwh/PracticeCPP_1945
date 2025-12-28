#pragma once
#include "SpriteActor.h"

class Sprite;
class Actor;

class Bullet : public SpriteActor
{
	using Super = SpriteActor;
public:
	Bullet(Actor* shotOwner, int dmg);
	virtual ~Bullet() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other) override;

	void SetBulletDamage(int value) { _damage = value; }
	int GetBulletDamage() { return _damage; }

	void SetDirection(Vec2 dir) { _direction = dir; }
	void SetSpeed(float speed) { _speed = speed; }

	void Reset(Actor* owner, int damage);
	void SetActive(bool active) { _active = active; }
	bool IsActive() const { return _active; }

private:
	LAYER_TYPE _ownerLayer = LAYER_OBJECT;
	Sprite* _sprite = nullptr;
	int _damage = 0;
	Vec2 _direction = { 0, -1 };	// 기본 : 위로 (플레이어용)
	float _speed = 300.f;

	bool _active = true;
};

