#pragma once
#include "Scene.h"
#include "Bitmap.h"
#include <vector>

class Car;
class BoxCollision;
class Player;

class Scene_Level final
	: public Scene
{
public:

	Scene_Level();
	virtual ~Scene_Level();

	virtual void Draw(const FVector2& windowSize) const override;

	virtual void Update(float deltaTime) override;

	Player* GetPlayer() const { return m_pPlayer; }

private:

	float m_AccumulatedTime{};

	Bitmap m_BackgroundSprites;

	std::vector<Car*> m_Cars;

	Player* m_pPlayer{};

	BoxCollision* m_pPlayerCollision{};

	FRect m_WaterBounds = { -32.f,0, 288.f, 120.f };

};

