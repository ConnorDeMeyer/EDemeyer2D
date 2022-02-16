#pragma once
#include "ICollisionComponent.h"
#include "Structs.h"
#include <vector>

class PolygonCollision final : public ICollisionComponent
{
public:
	PolygonCollision();
	virtual ~PolygonCollision() = default;

	virtual void Draw(const FVector2& windowRect) const override;

protected:

	std::vector<FPoint2> m_Vertices;
};

