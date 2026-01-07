#pragma once
#include "SpriteActor.h"

class Sprite;
class Collider;
class BoxCollider;

enum class BossState
{
    Waiting,
    Entering,
    Active,
};

enum class BossPattern
{
	Idle,
	Radial,
	Aimed,
};

class EnemyBoss : public SpriteActor
{
	using Super = SpriteActor;
public:
	EnemyBoss();
	virtual ~EnemyBoss() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other) override;
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other) override;

	void SetTargetPos(Vec2 pos) { _targetPos = pos; }
    void SetFinalPos(Vec2 pos) { _finalPos = pos; }
    void SetCollider(BoxCollider* collider) { _collider = collider; }

private:
    void UpdateState(float deltaTime);
    void UpdateWaiting(float deltaTime);
    void UpdateEntering(float deltaTime);
    void UpdateActive(float deltaTime);

	void UpdatePattern(float deltaTime);
	void RadialPattern(float deltaTime);
	void AimedPattern(float deltaTime);

	void ShootRadial();
	void ShootAtTarget(Vec2 from);
	void SpawnBullet(Vec2 pos, Vec2 dir);

private:
    Sprite* _sprite = nullptr;

    BossState _state = BossState::Waiting;
    float _waitTimer = 0.f;
    float _waitDuration = 30.f;

    Vec2 _startPos = { 0, 0 };
    Vec2 _finalPos = { 0, 0 };
    float _enterSpeed = 50.f;

    BossPattern _currentPattern = BossPattern::Idle;
    float _patternTimer = 0.f;
    float _patternDuration = 5.f;
    int _patternIndex = 0;

    float _rotationAngle = 0.f;
    float _rotationSpeed = 2.f;
    float _radialFireTimer = 0.f;
    float _radialFireInterval = 0.2f;
    int _radialBulletCount = 5;

    float _aimedFireTimer = 0.f;
    float _aimedFireInterval = 0.3f;
    Vec2 _leftWingOffset = { -80, 20 };
    Vec2 _rightWingOffset = { 80, 20 };
    bool _fireFromLeft = true;

    Vec2 _targetPos = { 300, 650 };
    int _damage = 5;

    BoxCollider* _collider = nullptr;
};

