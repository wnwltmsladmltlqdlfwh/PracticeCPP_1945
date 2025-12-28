#pragma once

class Collider;

class CollisionManager
{
	DECLARE_SINGLE(CollisionManager);

public:
	void Init();
	void Update();

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);
	void SetCollisionLayer(LAYER_TYPE layerA, LAYER_TYPE layerB, bool enable)
	{
		_collisionMatrix[layerA][layerB] = enable;
		_collisionMatrix[layerB][layerA] = enable;
	}

	void ToggleDebugRender() { _showDebugRender = !_showDebugRender; }
	bool IsDebugRenderEnable() { return _showDebugRender; }

private:
	void CheckCollisionBetweenLayers(LAYER_TYPE layerA, LAYER_TYPE layerB);

private:
	bool _collisionMatrix[LAYER_MAXCOUNT][LAYER_MAXCOUNT] = { false };
	vector<Collider*> _collidersByLayer[LAYER_MAXCOUNT];
	bool _showDebugRender = true;
};

