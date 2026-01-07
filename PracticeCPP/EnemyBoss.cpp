#include "pch.h"
#include "EnemyBoss.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "DevScene.h"
#include "Bullet.h"
#include "SphereCollider.h"
#include "BulletPool.h"
#include "BoxCollider.h"

EnemyBoss::EnemyBoss()
{
	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Enemy_Boss");
}

EnemyBoss::~EnemyBoss()
{
	for (Component* component : _components)
	{
		if (Collider* collider = dynamic_cast<Collider*>(component))
			GET_SINGLE(CollisionManager)->RemoveCollider(collider);
	}
}

void EnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	SetSprite(_sprite);

	_startPos = { _finalPos.x, -150.f };
	_pos = _startPos;

	_state = BossState::Waiting;
	_waitTimer = 0.f;
	_patternTimer = 0.f;
}

void EnemyBoss::Tick()
{
	Super::Tick();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	UpdateState(deltaTime);
}



void EnemyBoss::UpdateState(float deltaTime)
{
	switch (_state)
	{
	case BossState::Waiting:
		UpdateWaiting(deltaTime);
		break;
	case BossState::Entering:
		UpdateEntering(deltaTime);
		break;
	case BossState::Active:
		UpdateActive(deltaTime);
		break;
	}
}

void EnemyBoss::UpdateWaiting(float deltaTime)
{
	_waitTimer += deltaTime;

	if (_waitTimer >= _waitDuration)
	{
		_state = BossState::Entering;
		_pos = _startPos;
	}
}

void EnemyBoss::UpdateEntering(float deltaTime)
{
	Vec2 dir = _finalPos - _pos;
	float distance = sqrt(dir.x * dir.x + dir.y * dir.y);

	if (distance <= _enterSpeed * deltaTime)
	{
		_pos = _finalPos;
		_state = BossState::Active;
		_currentPattern = BossPattern::Radial;
		_patternTimer = 0.f;
		if (_collider)
		{
			GET_SINGLE(CollisionManager)->AddCollider(_collider);
		}
	}
	else
	{
		dir.Normalize();
		_pos.x += dir.x * _enterSpeed * deltaTime;
		_pos.y += dir.y * _enterSpeed * deltaTime;
	}
}

void EnemyBoss::UpdateActive(float deltaTime)
{
	UpdatePattern(deltaTime);
}

void EnemyBoss::UpdatePattern(float deltaTime)
{
	_patternTimer += deltaTime;

	if (_patternTimer >= _patternDuration)
	{
		_patternTimer = 0.f;
		_patternIndex = (_patternIndex + 1) % 2;

		if (_patternIndex == 0)
			_currentPattern = BossPattern::Radial;
		else
			_currentPattern = BossPattern::Aimed;
	}

	switch (_currentPattern)
	{
	case BossPattern::Radial:
		RadialPattern(deltaTime);
		break;
	case BossPattern::Aimed:
		AimedPattern(deltaTime);
		break;
	}
}

void EnemyBoss::RadialPattern(float deltaTime)
{
	_rotationAngle += _rotationSpeed * deltaTime;
	if (_rotationAngle >= 2 * PI)
		_rotationAngle -= 2 * PI;

	_radialFireTimer += deltaTime;
	if (_radialFireTimer >= _radialFireInterval)
	{
		ShootRadial();
		_radialFireTimer = 0.f;
	}
}

void EnemyBoss::AimedPattern(float deltaTime)
{
	_aimedFireTimer += deltaTime;
	if (_aimedFireTimer >= _aimedFireInterval)
	{
		if (_fireFromLeft)
		{
			ShootAtTarget(_pos + _leftWingOffset);
		}
		else
		{
			ShootAtTarget(_pos + _rightWingOffset);
		}
		_fireFromLeft = !_fireFromLeft;
		_aimedFireTimer = 0.f;
	}
}

void EnemyBoss::ShootRadial()
{
	float angleStep = (2 * PI) / _radialBulletCount;

	for (int i = 0; i < _radialBulletCount; i++)
	{
		float angle = _rotationAngle + (angleStep * i);
		Vec2 dir = { cos(angle), sin(angle) };

		SpawnBullet(_pos, dir);
	}
}

void EnemyBoss::ShootAtTarget(Vec2 from)
{
	Vec2 dir = _targetPos - from;
	dir.Normalize();

	SpawnBullet(from, dir);
}

void EnemyBoss::SpawnBullet(Vec2 pos, Vec2 dir)
{
	Bullet* bullet = GET_SINGLE(BulletPool)->Acquire(this, _damage);

	bullet->SetPos(pos);
	bullet->SetDirection(dir);
	bullet->SetSpeed(250.f);
	bullet->SetLayer(LAYER_BULLET);

	SphereCollider* collider = new SphereCollider();
	collider->SetRadius(6.f);
	bullet->AddComponent(collider);
	GET_SINGLE(CollisionManager)->AddCollider(collider);
}

void EnemyBoss::Render(HDC hdc)
{
	if (_state == BossState::Waiting)
		return;

	Super::Render(hdc);
}

void EnemyBoss::OnComponentBeginOverlap(Collider* collider, Collider* other)
{
}

void EnemyBoss::OnComponentEndOverlap(Collider* collider, Collider* other)
{
}
