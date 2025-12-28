#include "pch.h"
#include "HitEffect.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Flipbook.h"
#include "DevScene.h"
#include "Damagable.h"

HitEffect::HitEffect()
{
	_fb1 = GET_SINGLE(ResourceManager)->GetFlipbook(L"EnemyHit_FB_1");
	_fb2 = GET_SINGLE(ResourceManager)->GetFlipbook(L"EnemyHit_FB_2");
}

HitEffect::~HitEffect()
{
}

void HitEffect::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(_fb1);
	_curPhase = 0;
}

void HitEffect::Tick()
{
	Super::Tick();
	if (_finished || _flipbook == nullptr)
		return;

	const FlipbookInfo& info = _flipbook->GetInfo();
	int32 frameCount = (info.end - info.start + 1);

	if (info.loop == false && _idx >= frameCount - 1)
	{
		if (_curPhase == 0)
		{
			SetFlipbook(_fb2);
			_curPhase = 1;
		}
		else
		{
			_finished = true;
			RemovedSelf();
		}
	}
}

void HitEffect::Render(HDC hdc)
{
	Super::Render(hdc);
}
