#include "pch.h"
#include "BackgroundActor.h"
#include "Sprite.h"
#include "TimeManager.h"

BackgroundActor::BackgroundActor()
{
    SetLayer(LAYER_BACKGROUND);
}

BackgroundActor::~BackgroundActor()
{
}

void BackgroundActor::BeginPlay()
{
	Super::BeginPlay();

    if (_sprite == nullptr)
        return;

    Vec2Int srcSize = _sprite->GetSize();
    float ratio = (float)GWinSizeX / srcSize.x;
    _scaledHeight = (int32)(srcSize.y * ratio);

    _imageCount = (GWinSizeY / _scaledHeight) + 2;

    _yPositions.clear();
    for (int i = 0; i < _imageCount; i++)
    {
        _yPositions.push_back((float)(i * _scaledHeight) - _scaledHeight);
    }
}

void BackgroundActor::Tick()
{
	Super::Tick();

    if (_sprite == nullptr || _scaledHeight == 0)
        return;

    float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

    for (float& y : _yPositions)
    {
        y += _scrollSpeed * deltaTime;
    }

    for (float& y : _yPositions)
    {
        if (y >= GWinSizeY)
        {
            float minY = _yPositions[0];
            for (const float& otherY : _yPositions)
            {
                if (otherY < minY)
                    minY = otherY;
            }
            y = minY - _scaledHeight;
        }
    }
}

void BackgroundActor::Render(HDC hdc)
{
    if (_sprite == nullptr)
        return;

    Vec2Int srcSize = _sprite->GetSize();

    for (const float& y : _yPositions)
    {
        int32 drawY = (int32)y;

        if (drawY + _scaledHeight < 0 || drawY > GWinSizeY)
            continue;

        ::StretchBlt(hdc,
            0, drawY,
            GWinSizeX, _scaledHeight,
            _sprite->GetDC(),
            0, 0,
            srcSize.x, srcSize.y,
            SRCCOPY);
    }
}
