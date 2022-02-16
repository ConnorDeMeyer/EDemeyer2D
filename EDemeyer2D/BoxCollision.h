#pragma once

#include "ICollisionComponent.h"

class BoxCollision final : public ICollisionComponent
{
public:

	BoxCollision();
	virtual ~BoxCollision() = default;

	virtual void Draw(const FVector2& windowSize) const override;

	void SetBoundingRect(const FRect& rect) { m_BoundingRect = rect; }

	float GetWidth() { return m_BoundingRect.left - m_BoundingRect.right; }
	float GetHeight() { return m_BoundingRect.bottom - m_BoundingRect.top; }

};

