#include "pch.h"
#include "Damagable.h"
#include "Actor.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "HitEffect.h"

Damagable::Damagable()
{
}

Damagable::~Damagable()
{
}

void Damagable::BeginPlay()
{
	Super::BeginPlay();
}

void Damagable::TickComponent()
{
	Super::TickComponent();
}

void Damagable::Render(HDC hdc)
{
	Super::Render(hdc);
}

void Damagable::Damaged(int dmg)
{
	_stat.hp -= dmg;

	if (_stat.hp <= 0)
	{
		_stat.hp = 0;

		if (Actor* owner = GetOwner())
		{
			HitEffect* effect = new HitEffect();
			effect->SetLayer(LAYER_OBJECT);

			if (DevScene* devScene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene()))
			{
				if (owner->GetLayer() == LAYER_ENEMY)
				{
					devScene->OnEnemyKilled();
				}

      			devScene->AddActor(effect);
				effect->SetScale(3.f);
				effect->SetPos(owner->GetPos());
				devScene->RemoveActor(owner);
			}
		}
	}
}
