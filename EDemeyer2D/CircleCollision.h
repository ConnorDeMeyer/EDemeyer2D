#pragma once

#include "ICollisionComponent.h"

class CircleCollision final : public ICollisionComponent
{
public:
	CircleCollision();
	virtual ~CircleCollision();

	virtual void Draw(const FVector2& windowSize) const override;

	void SetRadius(float radius) { m_Radius = radius; CalculateBoundingBox(); }

	float GetRadius() const { return m_Radius; }

protected:

	void CalculateBoundingBox();
	

	float m_Radius{};
};

