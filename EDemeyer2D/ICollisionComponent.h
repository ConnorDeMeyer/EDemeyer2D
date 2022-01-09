#pragma once
#include "IComponent.h"
#include "Structs.h"
#include <forward_list>
#include "Delegates.h"

class ICollisionComponent : public IComponent
{
	friend class PhysicsManager;

public:
	ICollisionComponent() = default;
	virtual ~ICollisionComponent() = default;

	/** Get the bounding box relative to local origin*/
	const FRect& GetBoundingRect() const { return m_BoundingRect; }

	/** Get the bounding box relative to the world origin*/
	FRect GetWorldBoundingRect() const;

	IDelegate<IObject*> OnCollision;

	const std::forward_list<ICollisionComponent*>& GetOverlappingCollisions() { return m_OverlappingCollisions; }

protected:

	FRect m_BoundingRect = {};

private:
	
	std::forward_list<ICollisionComponent*> m_OverlappingCollisions;
};

