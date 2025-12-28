#include "pch.h"
#include "EnemySpawner_TypeB.h"
#include "Enemy_Normal.h"
#include "DevScene.h"
#include "CollisionManager.h"
#include "BoxCollider.h"
#include "Damagable.h"

EnemySpawner_TypeB::EnemySpawner_TypeB(DevScene* scene)
    : EnemySpawner(scene)
{
    SpawnLeftWave();
    SpawnRightWave();
    _firstSpawn = false;
}

EnemySpawner_TypeB::~EnemySpawner_TypeB()
{
}

void EnemySpawner_TypeB::Update(float deltaTime)
{
    for (Enemy_Normal* enemy : _activeEnemies)
    {
        if (enemy)
            enemy->SetTargetPos(_playerPos);
    }

    CleanupDeadEnemies();

    _spawnTimer += deltaTime;

    if (_spawnTimer >= _spawnInterval)
    {
        SpawnLeftWave();
        SpawnRightWave();
        _spawnTimer = 0.f;
    }
}

void EnemySpawner_TypeB::OnEnemyDead(Enemy_Normal* enemy)
{
    auto it = std::find(_activeEnemies.begin(), _activeEnemies.end(), enemy);
    if (it != _activeEnemies.end())
    {
        *it = nullptr;
    }
}

void EnemySpawner_TypeB::SpawnLeftWave()
{
    const int COUNT = 4;
    const float START_X = -40.f;
    const float START_Y = -50.f;
    const float SPACING_Y = -50.f;
    const float SPAWN_DELAY = 0.3f;

    Vec2 moveDir = { 1.f, 1.f };
    moveDir.Normalize();

    for (int i = 0; i < COUNT; i++)
    {
        Vec2 pos = { START_X - i * 30.f, START_Y + i * SPACING_Y };

        Enemy_Normal* enemy = CreateEnemy(pos, moveDir, 180.f);

        enemy->SetFireCondition(true);
        enemy->SetTargetPos(_playerPos);
        enemy->SetDamage(5);
        enemy->SetSpawner(this);

        _scene->AddActor(enemy);
        _activeEnemies.push_back(enemy);
    }
}

void EnemySpawner_TypeB::SpawnRightWave()
{
    const int COUNT = 4;
    const float START_X = GWinSizeX + 40.f;
    const float START_Y = -50.f;
    const float SPACING_Y = -50.f;

    Vec2 moveDir = { -1.f, 1.f };
    moveDir.Normalize();

    for (int i = 0; i < COUNT; i++)
    {
        Vec2 pos = { START_X + i * 30.f, START_Y + i * SPACING_Y };

        Enemy_Normal* enemy = CreateEnemy(pos, moveDir, 180.f);

        enemy->SetFireCondition(true);
        enemy->SetTargetPos(_playerPos);
        enemy->SetDamage(5);
        enemy->SetSpawner(this);

        _scene->AddActor(enemy);
        _activeEnemies.push_back(enemy);
    }
}

void EnemySpawner_TypeB::CleanupDeadEnemies()
{
    _activeEnemies.erase( std::remove(_activeEnemies.begin(), _activeEnemies.end(), nullptr), _activeEnemies.end() );
}
