#pragma once

#include <vector>
#include <set>

#include "Structs.h"

class ICollisionComponent;
class BoxCollision;
class CircleCollision;
class PolygonCollision;

#define PHYSICS PhysicsManager::GetInstance()

class PhysicsManager final
{
	friend class GameEngine;

private:

	PhysicsManager();
	~PhysicsManager();

	static PhysicsManager* m_Instance;

public:

	PhysicsManager(const PhysicsManager& other) = delete;
	PhysicsManager(PhysicsManager&& other) = delete;
	PhysicsManager& operator=(const PhysicsManager& other) = delete;
	PhysicsManager& operator=(PhysicsManager&& other) = delete;

	static PhysicsManager* GetInstance() { if (!m_Instance) m_Instance = new PhysicsManager(); return m_Instance; }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	void RegisterCollisionComp(ICollisionComponent* pComponent);

	void RemoveCollisionComp(ICollisionComponent* pComponent);

private:

	void PhysicsUpdate();

private:


	std::vector<BoxCollision*> m_BoxCollisions;
	std::vector<CircleCollision*> m_CircleCollisions;
	
	std::vector<ICollisionComponent*> m_CollisionComponents;
	
	/** This collision matrix will keep track of the objects that are colliding per frame
	* for every collision component that exists there is a row for it
	* if you want to know if 2 components are colliding you have to do [min][max]/[max][min] depending on the frame
	* there is also a backbuffer that you can access by doing [max][min]/[min][max] depending on the frame
	* to see if an collision component is registered at a space use [id][id] (returns false if there is none)*/
	std::vector<bool> m_CollisionMatrix;

	size_t m_LastMatPos{};

	size_t m_MatrixSize{};
	size_t m_CollisionComponentsAmount{};

	/** If on even frame the front buffer is accessed by using [max][min]
	* The back buffer is accessed using [min][max]*/
	bool m_EvenFrame{};

	void ResizeMatrix();

	void HandleCollision(ICollisionComponent* pComponent0, ICollisionComponent* pComponent1);

	//std::vector<PolygonCollision*> m_PolygonCollisions;

public:

	std::vector<ICollisionComponent*> GetOverlappingComponents(ICollisionComponent* pCollisionComp) const;

};