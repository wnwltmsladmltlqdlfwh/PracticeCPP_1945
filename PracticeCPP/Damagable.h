#pragma once
#include "Component.h";

class Damagable : public Component
{
	using Super = Component;
public:
	Damagable();
	virtual ~Damagable() override;

	virtual void BeginPlay() override;
	virtual void TickComponent() override;
	virtual void Render(HDC hdc) override;

	void SetStat(Stat newStat) { _stat = newStat; }
	Stat GetStat() { return _stat; }

	void Damaged(int dmg);
	bool IsDead() { return _stat.hp <= 0; }

private:
	Stat _stat;
};

