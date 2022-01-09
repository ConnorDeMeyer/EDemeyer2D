#include "LevelResources.h"
#include "Text.h"
#include "Texture.h"
#include "DrawingManager.h"
#include "BoxCollision.h"
#include "Frog.h"
#include "Player.h"
#include "Scene_Level.h"
#include "framework.h"

LevelResources::LevelResources()
{
	m_MaxTime = 120.f;
	m_RemainingTime = m_MaxTime;
	
	const tstring textureName{ _T("Resources/Bitmaps/Frogger_Misc.png") };
	const tstring fontName{ _T("Frogger_Font.ttf") };
	const DWRITE_FONT_WEIGHT weight{ DWRITE_FONT_WEIGHT_NORMAL };

	m_p1Up = new Text(_T("1-UP"), fontName, 12.f, weight);
	m_p1Up->SetColor(FColor{ 0,0.765f, 0.851f, 1.f });
	m_p1Up->SetLocalPosition({ 64.f,-2.f });
	AddComponent(m_p1Up);

	m_pHiScore = new Text(_T("HI-SCORE"), fontName, 12.f, weight);
	m_pHiScore->SetColor(FColor{ 0,0.765f, 0.851f, 1.f });
	m_pHiScore->SetLocalPosition({ 128.f,-2.f });
	AddComponent(m_pHiScore);

	m_pCurrentScore = new Text(_T("00000"), fontName, 12.f, weight);
	m_pCurrentScore->SetColor(FColor{ 0.878f, 0, 0, 1.f });
	m_pCurrentScore->SetLocalPosition({ 64.f,8 });
	AddComponent(m_pCurrentScore);

	m_pHiScoreValue = new Text(_T("00000"), fontName, 12.f, weight);
	m_pHiScoreValue->SetColor(FColor{ 0.878f, 0, 0, 1.f });
	m_pHiScoreValue->SetLocalPosition({ 128.f,8 });
	AddComponent(m_pHiScoreValue);

	m_pTimeText = new Text(_T("TIME"), fontName, 12.f, weight);
	m_pTimeText->SetColor(FColor{ 0.878f, 0.878f, 0, 1.f });
	m_pTimeText->SetLocalPosition({ 192.f,240.f });
	AddComponent(m_pTimeText);

	m_LiveTextures.reserve(m_Lives);
	Bitmap LiveTexture{ RESOURCES->LoadBitmap(textureName) };
	for (int i{}; i < m_Lives; ++i) {
		Texture* texture = new Texture(LiveTexture);
		texture->SetSourceRect(FRect{ 0,24,8,32 });
		texture->SetPosition({ i * 8.f + 4.f, 244.f });
		m_LiveTextures.push_back(texture);
		AddComponent(texture);
	}

	for (int i{}; i < 5; ++i)
	{
		auto frogGoal = new Texture(RESOURCES->LoadBitmap(textureName));
		frogGoal->SetSourceRect({ 0,48,16,64 });
		m_pFrogGoals[i] = frogGoal;
		AddComponent(frogGoal);

		auto collision = new BoxCollision();
		collision->SetBoundingRect({ -8,-8,8,8 });
		frogGoal->AddComponent(collision);

		frogGoal->SetPosition({ 16 + i * 48.f, 40.f });
		frogGoal->SetVisibility(false);
		collision->OnCollision.BindFunction(frogGoal, [frogGoal, this, collision](IObject* obj)
			{
				if (auto frog = dynamic_cast<Frog*>(obj)) {
					frogGoal->SetVisibility(true);
					this->ReachGoal();
					frog->Respawn();
					collision->SetActive(false);
				}
			});
	}

}

LevelResources::~LevelResources()
{
}

void LevelResources::Draw(const FVector2& windowRect) const
{
	// Draw Level Timer
	FRect barRect{ 96.f, 244.f, 190.f, 252.f };
	barRect.left -= (1 - max(m_RemainingTime / m_MaxTime, 0)) * (barRect.left - barRect.right);
	DRAW->FillRectangle(barRect, FColor{ 0.114f, 0.765f, 0, 1 });

	IObject::Draw(windowRect);
}

void LevelResources::Update(float deltaTime)
{
	IObject::Update(deltaTime);

	if (!m_bPauseTimer)
	{
		m_RemainingTime -= deltaTime;
		if (m_RemainingTime <= 0)
		{
			if (auto level = dynamic_cast<Scene_Level*>(ENGINE->GetCurrentScene()))
			{
				auto player = level->GetPlayer();
				player->Die();
			}
		}
	}
}

void LevelResources::LoseLife()
{
	if (m_Lives > 0)
	{
		--m_Lives;
		m_LiveTextures.back()->Destroy();
		m_LiveTextures.pop_back();
		ResetTimer();
		if (m_Lives == 0)
		{
			OnGameOver.BroadCast();
		}
	}
}

void LevelResources::ReachGoal()
{
	ResetTimer();

	for (int i{}; i < 5; ++i)
	{
		if (!m_pFrogGoals[i]->IsVisible()) return;
	}

	WinGame();
}

void LevelResources::WinGame()
{
	OnGameWin.BroadCast();

	for (int i{}; i < 5; ++i)
	{
		m_pFrogGoals[i]->SetSourceRect({ 16,48,32,64 });
		m_pFrogGoals[i]->SetActive(true);
	}

}
