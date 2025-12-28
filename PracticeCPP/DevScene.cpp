#include "pch.h"
#include "DevScene.h"
#include "Utils.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Actor.h"
#include "SpriteActor.h"
#include "Player.h"
#include "Flipbook.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "EnemyBase.h"
#include "EnemyBoss.h"
#include "Damagable.h"
#include "BackgroundActor.h"
#include "EnemySpawner_TypeA.h"
#include "EnemySpawner_TypeB.h"
#include "HealthBarUI.h"
#include "PowerUpItem.h"

DevScene::DevScene()
{

}

DevScene::~DevScene()
{
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			SAFE_DELETE(actor);

	_actors->clear();

	for (UI* ui : _uis)
		SAFE_DELETE(ui);

	_uis.clear();

	SAFE_DELETE(_spawnerA);
	SAFE_DELETE(_spawnerB);
}

void DevScene::Init()
{
	GET_SINGLE(CollisionManager)->SetCollisionLayer(LAYER_PLAYER, LAYER_BULLET, true);
	GET_SINGLE(CollisionManager)->SetCollisionLayer(LAYER_ENEMY, LAYER_BULLET, true);
	GET_SINGLE(CollisionManager)->SetCollisionLayer(LAYER_PLAYER, LAYER_ITEM, true);

	GET_SINGLE(ResourceManager)->LoadTexture(L"Player", L"Sprite\\Player\\airplane.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->LoadTexture(L"EnemyHit", L"Sprite\\Effect\\effect_enemy_hit.bmp", RGB(255, 91, 175));
	GET_SINGLE(ResourceManager)->LoadTexture(L"PowerUpItem", L"Sprite\\Projectile\\Missile.bmp", RGB(255, 0, 255));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Player_Bullet", GET_SINGLE(ResourceManager)->LoadTexture(L"Player_Bullet", L"C:Sprite\\Projectile\\player_bullet.bmp", RGB(255, 0, 255)));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Enemy_Bullet", GET_SINGLE(ResourceManager)->LoadTexture(L"Enemy_Bullet", L"C:Sprite\\Projectile\\bullet_r.bmp", RGB(255, 0, 255)));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Enemy_0", GET_SINGLE(ResourceManager)->LoadTexture(L"Enemy_0", L"C:Sprite\\Monster\\enemy0.bmp", RGB(255, 0, 255)));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Enemy_Boss", GET_SINGLE(ResourceManager)->LoadTexture(L"Enemy_Boss", L"C:Sprite\\Monster\\boss.bmp", RGB(255, 0, 255)));
	GET_SINGLE(ResourceManager)->CreateSprite(L"Background", GET_SINGLE(ResourceManager)->LoadTexture(L"Background", L"Sprite\\Background\\loopbg.bmp"));

	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Player");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"Player_Idle");
		fb->SetInfo({ texture, L"Player_Idle", {48, 49}, 12, 12, 0, 0.4f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Player");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"Player_Left");
		fb->SetInfo({ texture, L"Player_Left", {48, 49}, 0, 11, 0, 0.4f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"Player");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"Player_Right");
		fb->SetInfo({ texture, L"Player_Right", {48, 49}, 0, 11, 1, 0.4f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"EnemyHit");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"EnemyHit_FB_1");
		fb->SetInfo({ texture, L"EnemyHit_FB_1", {16, 16}, 0, 5, 0, 0.2f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"EnemyHit");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"EnemyHit_FB_2");
		fb->SetInfo({ texture, L"EnemyHit_FB_2", {16, 16}, 0, 5, 1, 0.2f, false });
	}
	{
		Texture* texture = GET_SINGLE(ResourceManager)->GetTexture(L"PowerUpItem");
		Flipbook* fb = GET_SINGLE(ResourceManager)->CreateFlipbook(L"PowerUp");
		fb->SetInfo({ texture, L"PowerUp", {32, 64}, 0, 12, 0, 0.2f, true });
	}

	{	// 배경 설정
		BackgroundActor* background = new BackgroundActor();
		background->SetSprite(GET_SINGLE(ResourceManager)->GetSprite(L"Background"));
		background->SetScrollSpeed(80.f);
		AddActor(background);
	}

	{
		EnemyBoss* boss = new EnemyBoss();
		boss->SetLayer(LAYER_ENEMY);
		boss->SetFinalPos({ GWinSizeX / 2, 200 });
		{
			BoxCollider* collider = new BoxCollider();
			collider->SetSize({ 185, 80 });
			boss->AddComponent(collider);
			GET_SINGLE(CollisionManager)->AddCollider(collider);

			Damagable* damagable = new Damagable();
			damagable->SetStat({ 50,50,0 });
			boss->AddComponent(damagable);
		}
		AddActor(boss);
		_boss = boss;
	}

	{
		Player* player = new Player();
		player->SetLayer(LAYER_PLAYER);
		{
			BoxCollider* collider = new BoxCollider();
			collider->SetSize({40, 50});
			player->AddComponent(collider);
			GET_SINGLE(CollisionManager)->AddCollider(collider);

			Damagable* damagable = new Damagable();
			damagable->SetStat({ 100,100,200 });
			player->AddComponent(damagable);

			player->SetDamage(10);
			player->SetPos({ GWinSizeX / 2, 650 });
			_player = player;
		}
		AddActor(player);
	}

	{
		HealthBarUI* healthBar = new HealthBarUI();
		healthBar->SetPlayer(_player);
		// 스프라이트가 있으면 설정 (없으면 단색 사각형으로 그려짐)
		// healthBar->SetBackgroundSprite(GET_SINGLE(ResourceManager)->GetSprite(L"HpBarDown"));
		// healthBar->SetFillSprite(GET_SINGLE(ResourceManager)->GetSprite(L"HpBarUp"));
		AddActor(healthBar);
		_healthBar = healthBar;
	}

	// 스포너 생성
	_spawnerA = new EnemySpawner_TypeA(this);
	_spawnerB = new EnemySpawner_TypeB(this);

	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->BeginPlay();
}

void DevScene::Update()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F1))
	{
		GET_SINGLE(CollisionManager)->ToggleDebugRender();
	}

	FlushPending();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (_player && _spawnerA)
	{
		_spawnerA->SetPlayerPos(_player->GetPos());
		_spawnerA->Update(deltaTime);
	}

	if (_player && _spawnerB)
	{
		_spawnerB->SetPlayerPos(_player->GetPos());
		_spawnerB->Update(deltaTime);
	}

	if (_boss && _player)
	{
		_boss->SetTargetPos(_player->GetPos());
	}

	// �Ÿ� = �ð� * �ӵ�
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->Tick();

	GET_SINGLE(CollisionManager)->Update();

	FlushPending();

	// 오브젝트 수 확인
	int totalActors = 0;
	for (const vector<Actor*>& actors : _actors)
		totalActors += actors.size();

	int bulletCount = _actors[LAYER_BULLET].size();

	OutputDebugStringA(std::format("Actors: {}, Bullets: {}\n", totalActors, bulletCount).c_str());
}

void DevScene::Render(HDC hdc)
{
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->Render(hdc);
}

void DevScene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	//_actors[actor->GetLayer()].push_back(actor);

	actor->SetLayer(std::clamp(actor->GetLayer(), LAYER_BACKGROUND, LAYER_UI));
	_pendingAdd[actor->GetLayer()].push_back(actor);
}

void DevScene::RemoveActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	/*vector<Actor*>& v = _actors[actor->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), actor), v.end());*/

	const LAYER_TYPE layer = actor->GetLayer();

	// 삭제 대기중이면 무시 (중복 삭제 방지)
	auto& pending = _pendingRemove[layer];
	if (std::find(pending.begin(), pending.end(), actor) != pending.end())
		return;

	_pendingRemove[layer].push_back(actor);
}

void DevScene::FlushPending()
{
	for (int layer = LAYER_BACKGROUND; layer < LAYER_MAXCOUNT; ++layer)
	{
		if (!_pendingAdd[layer].empty())
		{
			auto& dest = _actors[layer];
			dest.insert(dest.end(), _pendingAdd[layer].begin(), _pendingAdd[layer].end());

			for (Actor* actor : _pendingAdd[layer])
				actor->BeginPlay();

			_pendingAdd[layer].clear();
		}

		if (!_pendingRemove[layer].empty())
		{
			auto& dest = _actors[layer];
			for (Actor* actor : _pendingRemove[layer])
			{
				if (actor == _player)
				{
					_player = nullptr;
					if (_healthBar)
						_healthBar->SetPlayer(nullptr);
				}

				if (actor == _boss)
					_boss = nullptr;

				auto it = std::remove(dest.begin(), dest.end(), actor);
				dest.erase(it, dest.end());
				SAFE_DELETE(actor);
			}
			_pendingRemove[layer].clear();
		}
	}
}

void DevScene::OnEnemyKilled()
{
	if (_player->GetBulletCount() >= 3)
		return;

	_enemyKillCount++;

	if (_enemyKillCount >= 5)
	{
		_enemyKillCount = 0;

		PowerUpItem* item = new PowerUpItem();
		item->SetFlipbook(GET_SINGLE(ResourceManager)->GetFlipbook(L"PowerUp"));
		item->SetPos({ (float)(rand() % (GWinSizeX - 100) + 50), 0.f });  // 랜덤 X 위치

		BoxCollider* collider = new BoxCollider();
		collider->SetSize({ 20, 40 });
		item->AddComponent(collider);
		GET_SINGLE(CollisionManager)->AddCollider(collider);

		AddActor(item);
	}
}
