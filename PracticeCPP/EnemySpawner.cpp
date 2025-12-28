#include "pch.h"
#include "EnemySpawner.h"
#include "Enemy_Normal.h"
#include "DevScene.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "Damagable.h"

EnemySpawner::EnemySpawner(DevScene* scene) : _scene(scene)
{
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Update(float deltaTime)
{
}

Enemy_Normal* EnemySpawner::CreateEnemy(Vec2 pos, Vec2 moveDir, float speed)
{
    Enemy_Normal* enemy = new Enemy_Normal();
    enemy->SetLayer(LAYER_ENEMY);
    enemy->SetPos(pos);
    enemy->SetMovePattern(moveDir, speed);

    BoxCollider* collider = new BoxCollider();
    collider->SetSize({ 40, 50 });
    enemy->AddComponent(collider);
    GET_SINGLE(CollisionManager)->AddCollider(collider);

    Damagable* damagable = new Damagable();
    damagable->SetStat({ 30, 30, 0 });
    enemy->AddComponent(damagable);

    return enemy;
}
