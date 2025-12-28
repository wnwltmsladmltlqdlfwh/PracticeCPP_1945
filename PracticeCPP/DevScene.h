#pragma once
#include "Scene.h"

class Actor;
class GameObject;
class UI;

class EnemySpawner_TypeA;
class EnemySpawner_TypeB;
class Player;
class HealthBarUI;
class EnemyBoss;

class DevScene : public Scene
{
public:
	DevScene();
	virtual ~DevScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);
	void FlushPending();
	void OnEnemyKilled();

public:
	Player* GetPlayer() { return _player; }

public:
	vector<Actor*> _actors[LAYER_MAXCOUNT];
	vector<Actor*> _pendingAdd[LAYER_MAXCOUNT];
	vector<Actor*> _pendingRemove[LAYER_MAXCOUNT];
	vector<UI*> _uis;

private:
	Player* _player = nullptr;
	HealthBarUI* _healthBar = nullptr;
	EnemyBoss* _boss = nullptr;
	EnemySpawner_TypeA* _spawnerA = nullptr;
	EnemySpawner_TypeB* _spawnerB = nullptr;
	int _enemyKillCount = 0;
};

