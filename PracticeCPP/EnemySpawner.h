#pragma once

class Enemy_Normal;
class DevScene;

class EnemySpawner
{
public:
	EnemySpawner(DevScene* scene);
	virtual ~EnemySpawner();

	virtual void Update(float deltaTime) = 0;
	void SetPlayerPos(Vec2 pos) { _playerPos = pos; }

protected:
	Enemy_Normal* CreateEnemy(Vec2 pos, Vec2 moveDir, float speed);

protected:
	DevScene* _scene = nullptr;
	Vec2 _playerPos = { 300, 650 };
};

