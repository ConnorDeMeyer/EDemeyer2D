#include "WaterObstacle.h"
#include "BoxCollision.h"
#include "Frog.h"
#include "framework.h"

FVector2 WaterObstacle::HorizontalBounds{ -16.f, 256.f };

WaterObstacle::WaterObstacle()
{
	m_pCollision = new BoxCollision();
	m_pCollision->SetBoundingRect({ -3,-8,3,8 });
	AddComponent(m_pCollision);
}

void WaterObstacle::Update(float deltaTime)
{
	IObject::Update(deltaTime);

	Move(FVector2(m_MovementSpeed * deltaTime, 0));

	// Trim to world
	if (m_pCollision->GetWorldBoundingRect().left < HorizontalBounds.x)
		SetPosition({ HorizontalBounds.y - m_pCollision->GetBoundingRect().right + m_pCollision->GetBoundingRect().left, GetPosition().y });
	if (m_pCollision->GetWorldBoundingRect().right > HorizontalBounds.y)
		SetPosition({ HorizontalBounds.x + m_pCollision->GetBoundingRect().right - m_pCollision->GetBoundingRect().left, GetPosition().y });
}

void WaterObstacle::OnOverlapObject(IObject* other, ICollisionComponent* component, ICollisionComponent* otherComponent)
{
	if (Frog* pFrog = dynamic_cast<Frog*>(other))
	{
		if (!pFrog->IsLeaping())
		{
			pFrog->Move({ m_MovementSpeed * ENGINE->GetDeltaTime(),0 });
		}
	}
}
