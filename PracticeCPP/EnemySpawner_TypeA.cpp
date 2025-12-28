#include "pch.h"
#include "EnemySpawner_TypeA.h"
#include "Enemy_Normal.h"
#include "DevScene.h"
#include "Damagable.h"

EnemySpawner_TypeA::EnemySpawner_TypeA(DevScene* scene)
    : EnemySpawner(scene)
{
    SpawnWave();
}

EnemySpawner_TypeA::~EnemySpawner_TypeA()
{
}

void EnemySpawner_TypeA::Update(float deltaTime)
{
    for (Enemy_Normal* enemy : _enemies)
    {
        if (enemy)
            enemy->SetTargetPos(_playerPos);
    }

    if (_aliveCount <= 0)
    {
        if (!_waitingRespawn)
        {
            _waitingRespawn = true;
            _respawnTimer = 0.f;
        }
        else
        {
            _respawnTimer += deltaTime;
            if (_respawnTimer >= _respawnDelay)
            {
                SpawnWave();
                _waitingRespawn = false;
            }
        }
    }
}

void EnemySpawner_TypeA::OnEnemyDead(Enemy_Normal* enemy)
{
    auto it = std::find(_enemies.begin(), _enemies.end(), enemy);
    if (it != _enemies.end())
    {
        *it = nullptr;
        _aliveCount--;
    }
}

void EnemySpawner_TypeA::SpawnWave()
{
    _enemies.clear();

    const int COUNT = 5;
    const float START_X = 100.f;
    const float SPACING = 80.f;
    const float START_Y = 150.f;

    for (int i = 0; i < COUNT; i++)
    {
        Vec2 pos = { START_X + i * SPACING, START_Y };
        Enemy_Normal* enemy = CreateEnemy(pos, Vec2{ 1, 0 }, 100.f);

        enemy->SetMoveRange(50.f, GWinSizeX - 50.f);
        enemy->SetFireInterval(2.0f);
        enemy->SetFireCondition(false);
        enemy->SetDamage(10);
        enemy->SetSpawner(this);

        _scene->AddActor(enemy);
        _enemies.push_back(enemy);
    }

    _aliveCount = COUNT;
}

bool EnemySpawner_TypeA::AllDead()
{
    for (Enemy_Normal* enemy : _enemies)
    {
        if (enemy)
        {
            Damagable* dmg = enemy->GetComponent<Damagable>();
            if (dmg && !dmg->IsDead())
                return false;
        }
    }
    return true;
}