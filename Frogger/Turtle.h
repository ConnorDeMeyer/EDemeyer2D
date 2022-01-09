#pragma once
#include "WaterObstacle.h"

class Turtle final
	: public WaterObstacle
{
public:

	Turtle();
	virtual ~Turtle() = default;

	virtual void Update(float deltaTime) override;

	void SetDiving(bool value);

private:

	class Sprite* m_pSprite{};

	bool m_bDiving{};

	float m_accumulatedTime{};

};

