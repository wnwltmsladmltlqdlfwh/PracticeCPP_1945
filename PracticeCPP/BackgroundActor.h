#pragma once
#include "SpriteActor.h"

class BackgroundActor : public SpriteActor
{
	using Super = SpriteActor;
public:
	BackgroundActor();
	virtual ~BackgroundActor() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void SetScrollSpeed(float speed) { _scrollSpeed = speed; }

private:
	float _scrollSpeed = 100.f;
	float _scrollOffset = 0.f;

	std::vector<float> _yPositions;
	int32 _scaledHeight = 0;
	int _imageCount = 0;
};

