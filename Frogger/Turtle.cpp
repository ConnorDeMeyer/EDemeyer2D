#include "Turtle.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "framework.h"

Turtle::Turtle()
{
	m_pSprite = new Sprite();
	m_pSprite->SetBitmap(RESOURCES->LoadBitmap(_T("Resources/Bitmaps/Frogger_Obstacles.png")));
	m_pSprite->SetTotalFrames(3);
	m_pSprite->SetFrameDimension({ 16.f,16.f });
	m_pSprite->SetTimePerFrame(1.5f);
	m_pSprite->SetFrameOffset(9);
	m_pSprite->SetCurrentFrame(9);
	AddComponent(m_pSprite);

}

void Turtle::Update(float deltaTime)
{
	WaterObstacle::Update(deltaTime);

	m_accumulatedTime += deltaTime;
}

void Turtle::SetDiving(bool value)
{
	m_bDiving = value;
	if (m_bDiving)
		m_pSprite->SetTotalFrames(5); // TODO this will not show right
	else
		m_pSprite->SetTotalFrames(3);
}
