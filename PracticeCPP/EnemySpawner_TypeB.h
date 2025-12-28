#pragma once
#include "EnemySpawner.h"

class Enemy_Normal;

class EnemySpawner_TypeB : public EnemySpawner
{
public:
    EnemySpawner_TypeB(DevScene* scene);
    virtual ~EnemySpawner_TypeB();

    virtual void Update(float deltaTime) override;

    void OnEnemyDead(Enemy_Normal* enemy);

private:
    void SpawnLeftWave();
    void SpawnRightWave();
    void CleanupDeadEnemies();

private:
    float _spawnInterval = 5.0f;
    float _spawnTimer = 0.f;
    bool _firstSpawn = true;
    vector<Enemy_Normal*> _activeEnemies;
};

