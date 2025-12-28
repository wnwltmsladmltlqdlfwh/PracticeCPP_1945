#pragma once
#include "Actor.h"

class Sprite;
class Player;

class HealthBarUI : public Actor
{
    using Super = Actor;
public:
    HealthBarUI();
    virtual ~HealthBarUI() override;

    virtual void BeginPlay() override;
    virtual void Tick() override;
    virtual void Render(HDC hdc) override;

    void SetPlayer(Player* player) { _player = player; }
    void SetBackgroundSprite(Sprite* sprite) { _bgSprite = sprite; }
    void SetFillSprite(Sprite* sprite) { _fillSprite = sprite; }

private:
    Player* _player = nullptr;
    Sprite* _bgSprite = nullptr;
    Sprite* _fillSprite = nullptr;

    Vec2 _barPos = { 20, GWinSizeY - 50 };
    Vec2Int _barSize = { 200, 20 };
};

