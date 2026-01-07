#pragma once

class Actor;
class Bullet;

class BulletPool
{
	DECLARE_SINGLE(BulletPool);

public:
	void Init(int poolSize = 500);
	void Clear();

	Bullet* Acquire(Actor* owner, int damage);
	void Release(Bullet* bullet);

	void TickAll();
	void RenderAll(HDC hdc);

private:
	vector<Bullet*> _pool;
	vector<Bullet*> _activeList;
};

