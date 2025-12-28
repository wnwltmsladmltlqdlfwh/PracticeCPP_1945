#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "Bullet.h";
#include "DevScene.h"
#include "BoxCollider.h"
#include "Damagable.h"

Player::Player()
{
	_flipbookIdle = GET_SINGLE(ResourceManager)->GetFlipbook(L"Player_Idle");
	_flipbookLeft = GET_SINGLE(ResourceManager)->GetFlipbook(L"Player_Left");
	_flipbookRight = GET_SINGLE(ResourceManager)->GetFlipbook(L"Player_Right");

	/*CameraComponent* camera = new CameraComponent();
	AddComponent(camera);*/
}

Player::~Player()
{
	for (Component* component : _components)
	{
		if (Collider* collider = dynamic_cast<Collider*>(component))
			GET_SINGLE(CollisionManager)->RemoveCollider(collider);
	}
}

void Player::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(_flipbookIdle);
}

void Player::Tick()
{
	Super::Tick();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (GET_SINGLE(InputManager)->GetButton(KeyType::SpaceBar) && _delayBullet <= 0.f)
	{
		Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
		if (DevScene* devScene = dynamic_cast<DevScene*>(scene))
		{
			// 총알 개수만큼 발사
			for (int i = 0; i < _bulletCount; i++)
			{
				Bullet* bullet = new Bullet(this, _bulletDamage);

				float offsetX = 0.f;
				if (_bulletCount > 1)
				{
					float spread = 30.f;
					float startX = -spread * (_bulletCount - 1) / 2.f;
					offsetX = startX + spread * i;
				}

				bullet->SetPos(_pos + Vec2{ offsetX, -30.f });
				bullet->SetBulletDamage(10);
				bullet->SetLayer(LAYER_BULLET);
				devScene->AddActor(bullet);

				BoxCollider* collider = new BoxCollider();
				collider->SetSize({ 6, 20 });
				bullet->AddComponent(collider);
				GET_SINGLE(CollisionManager)->AddCollider(collider);
				bullet->BeginPlay();
			}
			_delayBullet = 0.2f;
		}

		//if (DevScene* devScene = dynamic_cast<DevScene*>(scene))
		//{
		//	Bullet* bullet = new Bullet(this, _bulletDamage);
		//	bullet->SetPos(_pos + Vec2{ 0.f, -30.f });   // 총알 시작 위치 보정
		//	bullet->SetBulletDamage(10);
		//	bullet->SetLayer(LAYER_BULLET);
		//	devScene->AddActor(bullet);	               // 기존 AddActor 로직 재활용
		//	BoxCollider* collider = new BoxCollider();
		//	collider->SetSize({ 6, 20 });
		//	bullet->AddComponent(collider);
		//	GET_SINGLE(CollisionManager)->AddCollider(collider);
		//	bullet->BeginPlay();
		//	_delayBullet = 0.2f;
		//}
	}
	else if (_delayBullet > 0.f)
	{
		_delayBullet -= deltaTime;
	}

	// TODO
	if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
	{
		_pos.y -= GetComponent<Damagable>()->GetStat().speed * deltaTime;
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
	{
		_pos.y += GetComponent<Damagable>()->GetStat().speed * deltaTime;
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::A))
	{
		_pos.x -= GetComponent<Damagable>()->GetStat().speed * deltaTime;
		SetReverse(true);
		SetFlipbook(_flipbookLeft);
	}
	if (GET_SINGLE(InputManager)->GetButton(KeyType::D))
	{
		_pos.x += GetComponent<Damagable>()->GetStat().speed * deltaTime;
		SetReverse(false);
		SetFlipbook(_flipbookRight);
	}

	if (GET_SINGLE(InputManager)->GetButton(KeyType::D) || GET_SINGLE(InputManager)->GetButton(KeyType::A))
		return;

	SetReverse(false);
	SetFlipbook(_flipbookIdle);
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);


}

void Player::OnComponentBeginOverlap(Collider* collider, Collider* other)
{

}

void Player::OnComponentEndOverlap(Collider* collider, Collider* other)
{

}
