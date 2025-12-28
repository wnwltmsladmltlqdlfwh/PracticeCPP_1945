#pragma once
#include "EnemyBase.h"

class EnemySpawner;

class Enemy_Normal : public EnemyBase
{
	using Super = EnemyBase;
public:
	Enemy_Normal();
	virtual ~Enemy_Normal() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other) override;
	
	void SetMovePattern(Vec2 direction, float speed) { _moveDir = direction, _speed = speed; }
	void SetMoveRange(float minX, float maxX) { _minX = minX, _maxX = maxX; }

	void SetFireInterval(float interval){ _fireInterval = interval; }
	void SetFireCondition(bool onHalfScreen) { _fireOnHalfScreen = onHalfScreen; }
	void SetDamage(int damage) { _damage = damage; }

	void SetTargetPos(Vec2 pos) { _targetPos = pos; }
	void SetSpawner(EnemySpawner* spawner) { _spawner = spawner; }

private:
	void ShootAtTarget();
	void UpdateMovement(float deltaTime);

private:
	EnemySpawner* _spawner = nullptr;
	Vec2 _moveDir = { 0,0 };
	float _speed = 100.f;
	float _minX = 50.f;
	float _maxX = 550.f;

	float _fireTimer = 0.f;
	float _fireInterval = 2.f;
	bool _hasFired = false;
	bool _fireOnHalfScreen = false;
	bool _enteredScreen = false;

	int _damage = 10;
	Vec2 _targetPos = { 300, 650 };
};

