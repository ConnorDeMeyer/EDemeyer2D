#include "PhysicsManager.h"
#include "BoxCollision.h"
#include "CircleCollision.h"
#include "PolygonCollision.h"
#include "ICollisionComponent.h"
#include "IObject.h"

PhysicsManager* PhysicsManager::m_Instance = nullptr;

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::RemoveCollisionComp(BoxCollision* boxColl)
{
	for (size_t i{}; i < m_BoxCollisions.size(); ++i)
		if (m_BoxCollisions[i] == boxColl)
		{
			m_BoxCollisions[i] = m_BoxCollisions.back();
			m_BoxCollisions.pop_back();
		}
}

void PhysicsManager::RemoveCollisionComp(CircleCollision* circleColl)
{
	for (size_t i{}; i < m_CircleCollisions.size(); ++i)
		if (m_CircleCollisions[i] == circleColl)
		{
			m_CircleCollisions[i] = m_CircleCollisions.back();
			m_CircleCollisions.pop_back();
		}
}

void PhysicsManager::PhysicsUpdate()
{
	// Collision check for box collisions
	const size_t& boxVectorSize{ m_BoxCollisions.size() };
	
	for (auto box : m_BoxCollisions) box->m_OverlappingCollisions.clear();

	for (size_t i{}; i < boxVectorSize; ++i)
	{
		if (!m_BoxCollisions[i]->IsActive()) continue;
		for (size_t j{ i + 1 }; j < boxVectorSize; ++j)
		{
			if (!m_BoxCollisions[j]->IsActive()) continue;
			// Check if overlapping
			if (CheckIfOverlapping(m_BoxCollisions[i], m_BoxCollisions[j]))
			{
				m_BoxCollisions[i]->GetOwner()->OnOverlapObject(m_BoxCollisions[j]->GetOwner(), m_BoxCollisions[i], m_BoxCollisions[j]);
				m_BoxCollisions[j]->GetOwner()->OnOverlapObject(m_BoxCollisions[i]->GetOwner(), m_BoxCollisions[j], m_BoxCollisions[i]);
				m_BoxCollisions[i]->m_OverlappingCollisions.push_front(m_BoxCollisions[j]);
				m_BoxCollisions[j]->m_OverlappingCollisions.push_front(m_BoxCollisions[i]);
				m_BoxCollisions[i]->OnCollision.BroadCast(m_BoxCollisions[j]->GetOwner());
				m_BoxCollisions[j]->OnCollision.BroadCast(m_BoxCollisions[i]->GetOwner());
			}
		}
	}
}



bool CheckIfOverlapping(const BoxCollision* box0, const BoxCollision* box1)
{
	FRect cBox0{ box0->GetWorldBoundingRect() };
	FRect cBox1{ box1->GetWorldBoundingRect() };

	return !(
		cBox1.left		>	cBox0.right		|| 
		cBox1.right		<	cBox0.left		|| 
		cBox1.top		>	cBox0.bottom	|| 
		cBox1.bottom	<	cBox0.top);
}

bool CheckIfOverlapping(const CircleCollision* circle0, const CircleCollision* circle1)
{
	float length = circle0->GetRadius() + circle1->GetRadius();
	FVector2 difference{ circle0->GetPosition() - circle1->GetPosition() };
	return length * length > (difference.x * difference.x + difference.y * difference.y);
}

bool CheckIfOverlapping(const PolygonCollision* poly0, const PolygonCollision* poly1)
{
	return false; // TODO
}

bool CheckIfOverlapping(const BoxCollision* box, const CircleCollision* circle)
{
	return false; // TODO
}

bool CheckIfOverlapping(const BoxCollision* box, const PolygonCollision* polygon)
{
	return false; // TODO
}

bool CheckIfOverlapping(const CircleCollision* circle, const PolygonCollision* polygon)
{
	return false; // TODO
}
