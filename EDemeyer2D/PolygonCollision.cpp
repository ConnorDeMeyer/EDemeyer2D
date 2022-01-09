#include "PolygonCollision.h"
#include "DrawingManager.h"

void PolygonCollision::Draw(const FVector2& windowRect) const
{
	ICollisionComponent::Draw(windowRect);

#if defined(_DEBUG) || defined(DEBUG)
	DRAW->DrawPolygon(m_Vertices);
#endif
}
