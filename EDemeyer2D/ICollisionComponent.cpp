#include "ICollisionComponent.h"
#include "PhysicsManager.h"

ICollisionComponent::ICollisionComponent()
{
	PHYSICS->RegisterCollisionComp(this);
}

ICollisionComponent::~ICollisionComponent()
{
	PHYSICS->RemoveCollisionComp(this);
}

FRect ICollisionComponent::GetWorldBoundingRect() const
{
	const FVector2& pos{ GetPosition() };
	return FRect{ pos.x + m_BoundingRect.left, pos.y + m_BoundingRect.top, pos.x + m_BoundingRect.right, pos.y + m_BoundingRect.bottom };
}

