#include "pch.h"
#include "EnemyBase.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "DevScene.h"
#include "Collider.h"

EnemyBase::EnemyBase()
{
	_sprite = GET_SINGLE(ResourceManager)->GetSprite(L"Enemy_0");
}

EnemyBase::~EnemyBase()
{
	for (Component* component : _components)
	{
		if (Collider* collider = dynamic_cast<Collider*>(component))
			GET_SINGLE(CollisionManager)->RemoveCollider(collider);
	}
}

void EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	SetSprite(_sprite);
}

void EnemyBase::Tick()
{
	Super::Tick();
}

void EnemyBase::Render(HDC hdc)
{
	Super::Render(hdc);
}

void EnemyBase::OnComponentBeginOverlap(Collider* collider, Collider* other)
{
	
}

void EnemyBase::OnComponentEndOverlap(Collider* collider, Collider* other)
{
}
