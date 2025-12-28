#include "pch.h"
#include "HealthBarUI.h"
#include "Sprite.h"
#include "Player.h"
#include "Damagable.h"

HealthBarUI::HealthBarUI()
{
    SetLayer(LAYER_UI);
}

HealthBarUI::~HealthBarUI()
{
}

void HealthBarUI::BeginPlay()
{
    Super::BeginPlay();
}

void HealthBarUI::Tick()
{
    Super::Tick();

    if (_player)
    {
        Damagable* damagable = _player->GetComponent<Damagable>();
        if (damagable == nullptr || damagable->IsDead())
        {
            _player = nullptr;
        }
    }
}

void HealthBarUI::Render(HDC hdc)
{
    Super::Render(hdc);

    if (_player == nullptr)
        return;

    Damagable* damagable = _player->GetComponent<Damagable>();
    if (damagable == nullptr)
        return;

    Stat stat = damagable->GetStat();
    float ratio = (float)stat.hp / stat.maxHp;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    int32 x = (int32)_barPos.x;
    int32 y = (int32)_barPos.y;

    if (_bgSprite)
    {
        Vec2Int srcSize = _bgSprite->GetSize();
        ::StretchBlt(hdc,
            x, y,
            _barSize.x, _barSize.y,
            _bgSprite->GetDC(),
            _bgSprite->GetPos().x, _bgSprite->GetPos().y,
            srcSize.x, srcSize.y,
            SRCCOPY);
    }
    else
    {
        HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
        RECT bgRect = { x, y, x + _barSize.x, y + _barSize.y };
        FillRect(hdc, &bgRect, redBrush);
        DeleteObject(redBrush);
    }

    int32 fillWidth = (int32)(_barSize.x * ratio);

    if (_fillSprite && fillWidth > 0)
    {
        Vec2Int srcSize = _fillSprite->GetSize();
        int32 srcFillWidth = (int32)(srcSize.x * ratio);

        ::StretchBlt(hdc,
            x, y,
            fillWidth, _barSize.y,
            _fillSprite->GetDC(),
            _fillSprite->GetPos().x, _fillSprite->GetPos().y,
            srcFillWidth, srcSize.y,
            SRCCOPY);
    }
    else if (fillWidth > 0)
    {
        HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
        RECT fillRect = { x, y, x + fillWidth, y + _barSize.y };
        FillRect(hdc, &fillRect, greenBrush);
        DeleteObject(greenBrush);
    }
}