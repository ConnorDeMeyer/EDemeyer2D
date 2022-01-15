#include "Frog.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "BoxCollision.h"
#include "GameInstance_Frogger.h"
#include "Collision.h"
#include "framework.h"
#include "AudioComponent.h"

Frog::Frog()
{
	m_pFrogTexture = new Texture(RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Frogs.png")));
	SetTextureDirection(eMovementDirection::up);
	AddComponent(m_pFrogTexture);

	m_pDeathSprite = new Sprite();
	m_pDeathSprite->SetBitmap(RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Death.png")));
	m_pDeathSprite->SetVisibility(false);
	m_pDeathSprite->SetFrameDimension({ 16,16 });
	m_pDeathSprite->SetTotalFrames(8);
	m_pDeathSprite->SetTimePerFrame(0.5);
	m_pDeathSprite->SetLooping(false);
	m_pDeathSprite->OnAnimationEnd.BindFunction(this, std::bind(&Frog::Respawn, this));
	m_pDeathSprite->Pause(true);
	AddComponent(m_pDeathSprite);

	m_pBoxCollision = new BoxCollision();
	m_pBoxCollision->SetBoundingRect({ -TILE_DIM * 0.5f + 3.f,-TILE_DIM * 0.5f + 3.f,TILE_DIM * 0.5f - 3.f,TILE_DIM * 0.5f - 3.f});
	AddComponent(m_pBoxCollision);

	m_pJumpSound = new AudioComponent(_T("Resources/Audio/sound-frogger-hop.wav"));
	AddComponent(m_pJumpSound);

	m_pDeathSound = new AudioComponent(_T("Resources/Audio/sound-frogger-squash.wav"));
	OnFrogDeath.BindFunction(this, [this]() {m_pDeathSound->Play(); });
	AddComponent(m_pDeathSound);

	SetDrawingOrder(10);

}

void Frog::Update(float deltaTime)
{
	IObject::Update(deltaTime);

	if (m_MoveCouldown > 0)
	{
		Move(m_Movement * deltaTime);
		if (m_MoveCouldown - deltaTime <= 0.f)
		{
			MoveToClosestRow();
			SetTextureDirection(m_CurrentDirection);
			if (m_Movement != FVector2{}) m_MoveCouldown = 0.2f;
			m_Movement = {};
			m_bLeaping = false;
		}
	}

	m_MoveCouldown -= deltaTime;
}

void Frog::MoveDirection(eMovementDirection direction)
{
	if (!m_bCanMove || m_MoveCouldown > 0.f) return;

	m_pJumpSound->Play();

	FVector2 direction2{};
	switch (direction)
	{
	case eMovementDirection::up:
		direction2.y = -1.f;
		break;
	case eMovementDirection::right:
		direction2.x = 1.f;
		break;
	case eMovementDirection::down:
		direction2.y = 1.f;
		break;
	case eMovementDirection::left:
		direction2.x = -1.f;
		break;
	}

	FPoint2 FuturePos{ GetPosition() + direction2 * TILE_DIM };
	if (IsOverlapping(FuturePos, m_WorldBounds))
	{
		m_Movement = direction2 * m_MovementSpeed;
		m_MoveCouldown = TILE_DIM / m_MovementSpeed;
		SetTextureDirection(direction, true);
		
		if (direction == eMovementDirection::down || direction == eMovementDirection::up)
			m_bLeaping = true;
	}
}

void Frog::Die()
{
	if (m_bImmune) return;

	m_pDeathSprite->Reset();
	m_pDeathSprite->SetVisibility(true);
	m_pFrogTexture->SetVisibility(false);
	m_bCanMove = false;
	m_bImmune = true;
	m_Movement = {};
	m_bSnapToTile = false;

	OnFrogDeath.BroadCast();
}

void Frog::Respawn()
{
	SetTextureDirection(eMovementDirection::up);
	m_pDeathSprite->SetVisibility(false);
	m_pFrogTexture->SetVisibility(true);
	SetPosition(m_SpawnPos);
	m_bCanMove = true;
	m_Movement = {};
	m_bImmune = false;
	m_bSnapToTile = true;
}

void Frog::SetTextureDirection(eMovementDirection direction, bool isLeaping)
{
	m_CurrentDirection = direction;

	FRect SrcRect{ 0,0,16,16 };
	switch (direction)
	{
	case eMovementDirection::down:
		SrcRect.bottom = 16.f;
		SrcRect.top = 32.f;
		break;
	case eMovementDirection::right:
		SrcRect = { 32.f,16.f,48.f,32.f };
		break;
	case eMovementDirection::left:
		SrcRect.left = 32.f;
		SrcRect.right = 48.f;
		break;
	}

	if (isLeaping)
	{
		SrcRect.left += 16.f;
		SrcRect.right += 16.f;
	}

	SrcRect.bottom += (UINT8)m_FrogType * 32.f;
	SrcRect.top += (UINT8)m_FrogType * 32.f;

	m_pFrogTexture->SetSourceRect(SrcRect);
}

void Frog::MoveToClosestRow()
{
	if (!m_bSnapToTile) return;

	float squarePosY{ GetPosition().y / TILE_DIM };

	SetPosition(FVector2(GetPosition().x, floorf(squarePosY) * TILE_DIM + TILE_DIM * 0.5f));
}

