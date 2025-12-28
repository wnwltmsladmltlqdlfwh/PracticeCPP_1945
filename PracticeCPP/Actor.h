#pragma once
#include "DevScene.h"

class Component;
class Collider;

class Actor
{
public:
	Actor();
	virtual ~Actor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetPos(Vec2 pos) { _pos = pos; }
	Vec2 GetPos() { return _pos; }

	void SetLayer(LAYER_TYPE layer) { _layer = layer; }
	LAYER_TYPE GetLayer() { return _layer; }

	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	template<typename T>
	T* GetComponent()
	{
		for (Component* component : _components)
		{
			T* found = dynamic_cast<T*>(component);
			if (found)
				return found;
		}
		return nullptr;
	}

	// OnCollisionEnter2D / OnCollisionExit2D
	virtual void OnComponentBeginOverlap(Collider* collider, Collider* other);
	virtual void OnComponentEndOverlap(Collider* collider, Collider* other);

protected:
	void RemovedSelf();

protected:
	Vec2 _pos = { 0, 0 };
	LAYER_TYPE _layer = LAYER_OBJECT;
	vector<Component*> _components;
};

