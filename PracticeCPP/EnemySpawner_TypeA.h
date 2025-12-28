#pragma once
#include "EnemySpawner.h"

class EnemySpawner_TypeA : public EnemySpawner
{
public:
	EnemySpawner_TypeA(DevScene* scene);
	virtual ~EnemySpawner_TypeA();

	virtual void Update(float deltaTime) override;

	void OnEnemyDead(Enemy_Normal* enemy);

private:
	void SpawnWave();
	bool AllDead();

private:
	vector<Enemy_Normal*> _enemies;
	int _aliveCount = 0;
	float _respawnDelay = 2.f;
	float _respawnTimer = 0.f;
	bool _waitingRespawn = false;
};

