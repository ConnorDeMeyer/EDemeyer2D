#include "CircleCollision.h"
#include "DrawingManager.h"
#include "PhysicsManager.h"

CircleCollision::CircleCollision()
{
	m_CollisionType = eCollisionType::circle;
}

void CircleCollision::Draw(const FVector2& windowSize) const
{
	ICollisionComponent::Draw(windowSize);

#if defined(_DEBUG) || defined(DEBUG)
	DRAW->DrawEllipse(FEllipse{GetPosition().x, GetPosition().y, m_Radius, m_Radius }, FColor{ 0,0,1,1 });
#endif
}

void CircleCollision::CalculateBoundingBox()
{
	FVector2 pos = GetPosition();
	m_BoundingRect = { pos.x - m_Radius,pos.y - m_Radius,pos.x + m_Radius,pos.y + m_Radius };
}
