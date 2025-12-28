#pragma once
#include "Actor.h"

class Flipbook;

class FlipbookActor : public Actor
{
	using Super = Actor;
public:
	FlipbookActor();
	virtual ~FlipbookActor() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetFlipbook(Flipbook* flipbook);
	void Reset();

	void SetScale(float scale = 1.f) { _scale = scale; }
	void SetReverse(bool reverse) { _reverse = reverse; }

protected:
	Flipbook* _flipbook = nullptr;
	float _sumTime = 0.f;
	int32 _idx = 0;
	float _scale = 1.f;
	bool _reverse = false;
};

