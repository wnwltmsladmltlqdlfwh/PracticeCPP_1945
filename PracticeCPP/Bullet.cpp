#include "pch.h"
#include "Bullet.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "BulletPool.h"
#include "DevScene.h"
#include "Player.h"
#include "Actor.h"
#include "Collider.h"
#include "Damagable.h"

Bullet::Bullet(Actor* shotOwner, int dmg)
{
	_damage = dmg;

	if (shotOwner)
	{
		_ownerLayer = shotOwner->GetLayer();

		if (_ownerLayer == LAYER_PLAYER)
		{
			SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Player_Bullet"));
		}
		else
		{
			SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Enemy_Bullet"));
		}
	}
}

Bullet::~Bullet()
{
	for (Component* component : _components)
	{
		if (Collider* collider = dynamic_cast<Collider*>(component))
			GET_SINGLE(CollisionManager)->RemoveCollider(collider);
	}
}

void Bullet::BeginPlay()
{
	Super::BeginPlay();
}

void Bullet::Tick()
{
	if (!_active)
		return;

	Super::Tick();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	
	_pos.x += _direction.x * _speed * deltaTime;
	_pos.y += _direction.y * _speed * deltaTime;

	if(_pos.x < 0 || _pos.x > 600 || _pos.y < 0 || _pos.y > 800)
	{
		GET_SINGLE(BulletPool)->Release(this);
		//RemovedSelf();
		return;
	}
}

void Bullet::Render(HDC hdc)
{
	if (!_active)
		return;

	Super::Render(hdc);
}

void Bullet::OnComponentBeginOverlap(Collider* collider, Collider* other)
{
	if (!_active)
		return;

	Actor* otherActor = other->GetOwner();
	if (otherActor == nullptr)
		return;

	LAYER_TYPE otherLayer = otherActor->GetLayer();
	if (_ownerLayer == LAYER_PLAYER && otherLayer == LAYER_PLAYER)
		return;
	if (_ownerLayer == LAYER_ENEMY && otherLayer == LAYER_ENEMY)
		return;

	Damagable* damagable = otherActor->GetComponent<Damagable>();
	if (damagable)
	{
		damagable->Damaged(_damage);
		GET_SINGLE(BulletPool)->Release(this);
		//RemovedSelf();
	}
}

void Bullet::OnComponentEndOverlap(Collider* collider, Collider* other)
{

}

void Bullet::Reset(Actor* owner, int damage)
{
	_damage = damage;
	_active = true;
	_speed = 300.f;
	_direction = { 0,-1 };

	vector<Component*> collidersToRemove;
	for (Component* comp : _components)
	{
		if (Collider* col = dynamic_cast<Collider*>(comp))
		{
			GET_SINGLE(CollisionManager)->RemoveCollider(col);
			collidersToRemove.push_back(comp);
		}
	}
	for (Component* comp : collidersToRemove)
	{
		RemoveComponent(comp);
		delete comp;
	}

	if (owner)
	{
		_ownerLayer = owner->GetLayer();
		if (_ownerLayer == LAYER_PLAYER)
			SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Player_Bullet"));
		else
			SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Enemy_Bullet"));
	}
}
