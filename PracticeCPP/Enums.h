#pragma once

// Enums �ڷ� ����

enum class SceneType
{
	None,
	DevScene,
	GameScene,
	EditScene
};

enum LAYER_TYPE
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_ITEM,
	LAYER_ENEMY,
	LAYER_BULLET,
	LAYER_PLAYER,
	LAYER_UI,

	LAYER_MAXCOUNT
};

enum class ColliderType
{
	Box,
	Sphere,
};