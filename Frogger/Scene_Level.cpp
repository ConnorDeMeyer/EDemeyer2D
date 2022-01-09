#include "Scene_Level.h"
#include "ResourceManager.h"
#include "LevelResources.h"
#include "Car.h"
#include "Player.h"
#include "FileLoaders.h"
#include "BoxCollision.h"
#include "Collision.h"
#include "framework.h"

Scene_Level::Scene_Level()
{
	m_BackgroundSprites = RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Misc.png"));

	auto levelResource{ new LevelResources() };
	AddObjectToScene(levelResource);

	m_pPlayer = new Player();
	m_pPlayer->SetPosition(FVector2{ 112, 232 });
	m_pPlayerCollision = m_pPlayer->GetComponent<BoxCollision>();
	AddObjectToScene(m_pPlayer);

	std::function<void()> bindedFunction = std::bind(&LevelResources::LoseLife, levelResource);
	m_pPlayer->OnFrogDeath.BindFunction(levelResource, bindedFunction);

	LoadLevel(1, this);
}

Scene_Level::~Scene_Level()
{
}

void Scene_Level::Draw(const FVector2& windowSize) const
{

	// Draw background
	DRAW->FillRectangle(FRect{ 0,0,windowSize.x,windowSize.y * 0.5f }, FColor{ 0,0,0.278f,1 });
	DRAW->FillRectangle(FRect{ 0,windowSize.y * 0.5f,windowSize.x,windowSize.y * 0.5f }, FColor{ 0,0,0,1 });

	const FRect blueGrassSrc{ 48.f, 0.f,64.f,16.f };
	const FRect greenGrassSrc{ 0,0,48.f,24.f };

	// Draw grass
	for (int i{}; i < 5; ++i)
		DRAW->DrawBitmap(m_BackgroundSprites, FRect{ i * 48.f, 24.f, (i + 1) * 48.f,48.f }, greenGrassSrc);
	for (int i{}; i < 14; ++i) {
		DRAW->DrawBitmap(m_BackgroundSprites, FRect{ i * 16.f, 128.f, (i + 1) * 16.f,144.f }, blueGrassSrc);
		DRAW->DrawBitmap(m_BackgroundSprites, FRect{ i * 16.f, 224.f, (i + 1) * 16.f,240.f }, blueGrassSrc);
	}

	Scene::Draw(windowSize);
}

void Scene_Level::Update(float deltaTime)
{
	Scene::Update(deltaTime);

	m_AccumulatedTime += deltaTime;

	if (m_pPlayerCollision && IsOverlapping(m_pPlayerCollision->GetWorldBoundingRect(), m_WaterBounds))
	{
		auto collisions = m_pPlayerCollision->GetOverlappingCollisions();
		bool killPlayer{true};
		for (auto collision : collisions)
		{
			if (dynamic_cast<WaterObstacle*>(collision->GetOwner()))
			{
				killPlayer = false;
				break;
			}
		}
		if (killPlayer) m_pPlayer->Die();
	}
}
