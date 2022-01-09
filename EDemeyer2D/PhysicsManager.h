#pragma once

#include <vector>
#include "Structs.h"

class ICollisionComponent;
class BoxCollision;
class CircleCollision;
class PolygonCollision;

#define PHYSICS PhysicsManager::GetInstance()

class PhysicsManager
{
	friend class GameEngine;

private:

	PhysicsManager();
	~PhysicsManager();

	PhysicsManager(const PhysicsManager& other) = delete;
	PhysicsManager(PhysicsManager&& other) = delete;
	PhysicsManager& operator=(const PhysicsManager& other) = delete;
	PhysicsManager& operator=(PhysicsManager&& other) = delete;

	static PhysicsManager* m_Instance;

public:

	static PhysicsManager* GetInstance() { if (!m_Instance) m_Instance = new PhysicsManager(); return m_Instance; }
	static void Destroy() { delete m_Instance; m_Instance = nullptr; }

	void RegisterCollisionComp(BoxCollision* boxColl) { m_BoxCollisions.push_back(boxColl); }
	void RegisterCollisionComp(CircleCollision* circleColl) { m_CircleCollisions.push_back(circleColl); };

	void RemoveCollisionComp(BoxCollision* boxColl);
	void RemoveCollisionComp(CircleCollision* circleColl);

private:

	void PhysicsUpdate();

private:

	std::vector<BoxCollision*> m_BoxCollisions;
	std::vector<CircleCollision*> m_CircleCollisions;
	std::vector<PolygonCollision*> m_PolygonCollisions;

};

bool CheckIfOverlapping(const BoxCollision* box0,		const BoxCollision* box1);
bool CheckIfOverlapping(const CircleCollision* circle0, const CircleCollision* circle1);
bool CheckIfOverlapping(const PolygonCollision* poly0,	const PolygonCollision* poly1);

bool CheckIfOverlapping(const BoxCollision* box0,		const CircleCollision* box1);
bool CheckIfOverlapping(const BoxCollision* circle0,	const PolygonCollision* circle1);
bool CheckIfOverlapping(const CircleCollision* poly0,	const PolygonCollision* poly1);