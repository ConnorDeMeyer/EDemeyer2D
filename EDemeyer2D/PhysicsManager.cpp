#include "PhysicsManager.h"
#include "BoxCollision.h"
#include "CircleCollision.h"
#include "PolygonCollision.h"
#include "ICollisionComponent.h"
#include "IObject.h"
#include "Collision.h"

#include <stdexcept>

PhysicsManager* PhysicsManager::m_Instance = nullptr;

PhysicsManager::PhysicsManager()
	: m_MatrixSize{128}
{
	m_CollisionMatrix = std::vector<bool>(m_MatrixSize * m_MatrixSize, false);

	m_CollisionComponents.resize(m_MatrixSize);
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::RegisterCollisionComp(ICollisionComponent* pComponent)
{
	// check if [m_LastMatPos][m_LastMatPos] is valid
	while (m_CollisionMatrix[m_LastMatPos + m_MatrixSize * m_LastMatPos])
	{
		if (m_LastMatPos >= m_MatrixSize)
		{
			ResizeMatrix();
		}
		else
			++m_LastMatPos;
	}

	pComponent->m_Id = m_LastMatPos;
	m_CollisionMatrix[m_LastMatPos + m_MatrixSize * m_LastMatPos] = true;

	m_CollisionComponents[m_LastMatPos] = pComponent;

	++m_LastMatPos;
	++m_CollisionComponentsAmount;
}

void PhysicsManager::RemoveCollisionComp(ICollisionComponent* pComponent)
{
	size_t id = pComponent->m_Id;
	pComponent->m_Id = 0;
	for (size_t i{}; i < m_MatrixSize; ++i)
	{
		m_CollisionMatrix[id + m_MatrixSize * i] = false;
		m_CollisionMatrix[i + m_MatrixSize * id] = false;
	}

	m_CollisionComponents[id] = nullptr;
	--m_CollisionComponentsAmount;
}

void PhysicsManager::PhysicsUpdate()
{
	// Collision check for box collisions
	const size_t& boxVectorSize{ m_CollisionComponents.size() };
	
	//for (auto box : m_BoxCollisions) box->m_OverlappingCollisions.clear();

	size_t iCounter{};

	for (size_t i{}; i < boxVectorSize; ++i)
	{
		ICollisionComponent* pComponent0{ m_CollisionComponents[i] };
		if (!pComponent0 || !pComponent0->IsActive()) continue;
		
		++iCounter;
		if (iCounter >= m_CollisionComponentsAmount) break;

		size_t jCounter{};
		for (size_t j{ i + 1 }; j < boxVectorSize; ++j)
		{
			ICollisionComponent* pComponent1{ m_CollisionComponents[j] };
			if (!pComponent1 || !pComponent1->IsActive()) continue;

			++jCounter;
			if (jCounter >= m_CollisionComponentsAmount) break;

			HandleCollision(pComponent0, pComponent1);
		}
	}

	m_EvenFrame = !m_EvenFrame;
}

void PhysicsManager::ResizeMatrix()
{
	// first check if there are any empty spaces in between the matrix
	for (size_t i{}; i < m_MatrixSize; ++i)
	{
		if (!m_CollisionMatrix[m_MatrixSize * i + i])
		{
			m_LastMatPos = i;
			return;
		}
	}

	// multiply the size of the array by 2
	size_t newSize{ m_MatrixSize * 2 };

	std::vector<bool> oldVector = m_CollisionMatrix;
	m_CollisionMatrix = std::vector<bool>(newSize * newSize);

	for (size_t x{}; x < m_MatrixSize; ++x)
		for (size_t y{}; y < m_MatrixSize; ++y)
			m_CollisionMatrix[x + y * newSize] = oldVector[x + y * m_MatrixSize];

	m_CollisionComponents.resize(newSize);

	m_MatrixSize = newSize;
}

void PhysicsManager::HandleCollision(ICollisionComponent* pComponent0, ICollisionComponent* pComponent1)
{
	// Return if have the same owner
	if (pComponent0->GetOwner() == pComponent1->GetOwner()) return;

	size_t minId{ min(pComponent0->m_Id, pComponent1->m_Id) };
	size_t maxId{ max(pComponent0->m_Id, pComponent1->m_Id) };

	if (m_EvenFrame) std::swap(minId, maxId);

	UINT8 switchCase = UINT8(pComponent0->m_CollisionType) | UINT8(pComponent1->m_CollisionType);

	bool newValue{};

	if (IsOverlapping(pComponent0->GetWorldBoundingRect(), pComponent1->GetWorldBoundingRect()))
	{
		switch (switchCase)
		{
		case 0b1: // both are rects
			newValue = true;
			break;
		case 0b10: // both are circles
			newValue = IsOverlapping(
				FCircle{ pComponent0->GetPosition(), reinterpret_cast<CircleCollision*>(pComponent0)->GetRadius() },
				FCircle{ pComponent1->GetPosition(), reinterpret_cast<CircleCollision*>(pComponent1)->GetRadius() });
			break;
		case 0b100: // both are polygons
			//TODO
			break;
		case 0b11: // one is box other is circle
		{
			CircleCollision* pCircleCollision = (pComponent0->m_CollisionType == eCollisionType::circle) ?
				reinterpret_cast<CircleCollision*>(pComponent0) : reinterpret_cast<CircleCollision*>(pComponent1);
			BoxCollision* pBoxCollision = (pComponent0->m_CollisionType == eCollisionType::circle) ?
				reinterpret_cast<BoxCollision*>(pComponent1) : reinterpret_cast<BoxCollision*>(pComponent0);
			newValue = IsOverlapping(pBoxCollision->GetWorldBoundingRect(), FCircle{ pCircleCollision->GetPosition(), pCircleCollision->GetRadius() });
		}
			break;
		case 0b110: // one is circle other is polygon
			// TODO
			break;
		case 0b101: // one is box other is polygon
			// TODO
			break;
		default:
			throw std::invalid_argument("collision type is wrong");
			break;
		}
	}

	// get previous value
	bool previousValue{ m_CollisionMatrix[maxId * m_MatrixSize + minId] };

	if (previousValue != newValue && minId == 5)
	{
		int i{};
	}

	// update new value
	m_CollisionMatrix[minId * m_MatrixSize + maxId] = newValue;

	// send the functions
	if (previousValue != newValue)
	{
		IObject* pObject0 = pComponent0->GetOwner();
		IObject* pObject1 = pComponent1->GetOwner();

		if (newValue)
		{
			pObject0->OnBeginOverlap(pObject1);
			pObject1->OnBeginOverlap(pObject0);
		}
		else
		{
			pObject0->OnEndOverlap(pObject1);
			pObject1->OnEndOverlap(pObject0);
		}
	}
}

std::vector<ICollisionComponent*> PhysicsManager::GetOverlappingComponents(ICollisionComponent* pCollisionComp) const
{
	std::vector<ICollisionComponent*> Components;
	size_t id = pCollisionComp->m_Id;

	for (size_t i{}; i < m_MatrixSize; ++i)
	{
		size_t minId{ min(id, i) };
		size_t maxId{ max(id,i) };

		if (m_EvenFrame) std::swap(minId, maxId);

		if (m_CollisionMatrix[minId * m_MatrixSize + maxId] && minId != maxId)
			Components.push_back(m_CollisionComponents[i]);
	}

	return Components;
}
