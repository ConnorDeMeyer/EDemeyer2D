#include "BoxCollision.h"
#include "DrawingManager.h"
#include "PhysicsManager.h"

BoxCollision::BoxCollision()
{
	PHYSICS->RegisterCollisionComp(this);
}

BoxCollision::~BoxCollision()
{
	PHYSICS->RemoveCollisionComp(this);
}

void BoxCollision::Draw(const FVector2& windowSize) const
{
	ICollisionComponent::Draw(windowSize);

#if defined(_DEBUG) || defined(DEBUG)
	DRAW->DrawRectangle(GetWorldBoundingRect(), FColor{ 0,0,1,1 });
#endif
}
