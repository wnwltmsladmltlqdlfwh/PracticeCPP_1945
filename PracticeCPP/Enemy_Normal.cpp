#include "pch.h"
#include "Enemy_Normal.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "DevScene.h"
#include "Bullet.h"
#include "SphereCollider.h"
#include "EnemySpawner_TypeA.h"
#include "EnemySpawner_TypeB.h"

Enemy_Normal::Enemy_Normal()
{
	
}

Enemy_Normal::~Enemy_Normal()
{
	if (auto* spawnerA = dynamic_cast<EnemySpawner_TypeA*>(_spawner))
	{
		spawnerA->OnEnemyDead(this);
	}
	else if (auto* spawnerB = dynamic_cast<EnemySpawner_TypeB*>(_spawner))
	{
		spawnerB->OnEnemyDead(this);
	}
}

void Enemy_Normal::BeginPlay()
{
	Super::BeginPlay();
}

void Enemy_Normal::Tick()
{
	Super::Tick();
	
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	UpdateMovement(deltaTime);

	if (_fireOnHalfScreen)
	{
		if (!_hasFired && _pos.y >= GWinSizeY / 2)
		{
			ShootAtTarget();
			_hasFired = true;
		}
	}
	else
	{
		_fireTimer += deltaTime;
		if (_fireTimer >= _fireInterval)
		{
			ShootAtTarget();
			_fireTimer = 0.f;
		}
	}

	if (!_enteredScreen)
	{
		if (_pos.x >= 0 && _pos.x <= GWinSizeX && _pos.y >= 0 && _pos.y <= GWinSizeY)
		{
			_enteredScreen = true;
		}
	}
	else
	{
		if (_pos.y > GWinSizeY + 50 || _pos.x < -50 || _pos.x > GWinSizeX + 50)
		{
			RemovedSelf();
		}
	}
}

void Enemy_Normal::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Enemy_Normal::OnComponentBeginOverlap(Collider* collider, Collider* other)
{

}

void Enemy_Normal::OnComponentEndOverlap(Collider* collider, Collider* other)
{

}

void Enemy_Normal::ShootAtTarget()
{
	Vec2 dir = _targetPos - _pos;
	dir.Normalize();

	Bullet* bullet = new Bullet(this, _damage);
	bullet->SetPos(_pos + Vec2{ 0, 30 });
	bullet->SetDirection(dir);
	bullet->SetSpeed(200.f);
	bullet->SetLayer(LAYER_BULLET);

	SphereCollider* collider = new SphereCollider();
	collider->SetRadius(6.f);
	bullet->AddComponent(collider);
	GET_SINGLE(CollisionManager)->AddCollider(collider);

	if (DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene()))
	{
		scene->AddActor(bullet);
	}
}

void Enemy_Normal::UpdateMovement(float deltaTime)
{
	_pos.x += _moveDir.x * _speed * deltaTime;
	_pos.y += _moveDir.y * _speed * deltaTime;

	if (_moveDir.y == 0)
	{
		if (_pos.x <= _minX)
		{
			_pos.x = _minX;
			_moveDir.x = 1;
		}
		else if (_pos.x >= _maxX)
		{
			_pos.x = _maxX;
			_moveDir.x = -1;
		}
	}
}
