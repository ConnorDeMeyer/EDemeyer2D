#include "Car.h"
#include "ResourceManager.h"
#include "BoxCollision.h"
#include "Frog.h"
#include "framework.h"

FVector2 Car::HorizontalBounds{ -32.f, 272.f };

Car::Car()
{
	m_pTexture = new Texture(RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Obstacles.png")));
	m_pTexture->SetSourceRect({ 0,0,16,16 });
	AddComponent(m_pTexture);

	m_Collider = new BoxCollision();
	m_Collider->SetBoundingRect({ -8.f, -8.f, 8.f, 8.f });
	AddComponent(m_Collider);
}

void Car::Update(float deltaTime)
{
	IObject::Update(deltaTime);

	Move(FVector2(m_MovementSpeed * deltaTime, 0));

	// Trim to world
	if (m_Collider->GetWorldBoundingRect().left < HorizontalBounds.x)
		SetPosition({ HorizontalBounds.y - m_Collider->GetBoundingRect().right + m_Collider->GetBoundingRect().left, GetPosition().y });
	if (m_Collider->GetWorldBoundingRect().right > HorizontalBounds.y)
		SetPosition({ HorizontalBounds.x + m_Collider->GetBoundingRect().right - m_Collider->GetBoundingRect().left, GetPosition().y });
}

void Car::OnOverlapObject(IObject* other, ICollisionComponent* component, ICollisionComponent* otherComponent)
{
	IObject::OnOverlapObject(other, component, otherComponent);

	if (auto pPlayer = dynamic_cast<Frog*>(other))
	{
		pPlayer->Die();
	}
}

void Car::SetCarType(eCarType carType)
{
	switch (carType)
	{
	case eCarType::CarLeft1:
		m_pTexture->SetSourceRect(FRect{ 0,0,16,16 });
		break;
	case eCarType::CarLeft2:
		m_pTexture->SetSourceRect(FRect{ 16,0,32,16 });
		break;
	case eCarType::CarRight1:
		m_pTexture->SetSourceRect(FRect{ 32,0,48,16 });
		break;
	case eCarType::CarRight2:
		m_pTexture->SetSourceRect(FRect{ 48,0,64,16 });
		break;
	case eCarType::CarLeftBig:
		m_pTexture->SetSourceRect(FRect{ 0,16,32,32 });
		break;
	}
}
