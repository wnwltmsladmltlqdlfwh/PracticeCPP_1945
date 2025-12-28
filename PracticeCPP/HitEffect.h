#pragma once
#include "FlipbookActor.h"

class Flipbook;

class HitEffect : public FlipbookActor
{
	using Super = FlipbookActor;
public:
	HitEffect();
	virtual ~HitEffect() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

private:
	int _curPhase = 0;
	bool _finished = false;
	Flipbook* _fb1 = nullptr;
	Flipbook* _fb2 = nullptr;
};

