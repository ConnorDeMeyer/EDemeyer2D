#pragma once
#include "IComponent.h"
#include "Structs.h"
#include <forward_list>
#include "Delegates.h"

enum class eCollisionType : UINT8
{
	none = 0b0,
	box = 0b1,
	circle = 0b10,
	polygon = 0b100
};

class ICollisionComponent : public IComponent
{
	friend class PhysicsManager;

public:
	ICollisionComponent();
	virtual ~ICollisionComponent();

	/** Get the bounding box relative to local origin*/
	const FRect& GetBoundingRect() const { return m_BoundingRect; }

	/** Get the bounding box relative to the world origin*/
	FRect GetWorldBoundingRect() const;

	IDelegate<IObject*> OnCollision;

	size_t GetId() const { return m_Id; }

protected:

	FRect m_BoundingRect = {};
	eCollisionType m_CollisionType{};

private:
	
	size_t m_Id{};

};

