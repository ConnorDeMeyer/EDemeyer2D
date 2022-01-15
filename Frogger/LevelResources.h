#pragma once
#include "IObject.h"
#include "Bitmap.h"
#include "Text.h"
#include "Delegates.h"

class Texture;

class LevelResources final : public IObject
{
public:
	
	LevelResources();
	virtual ~LevelResources();

	virtual void Draw(const FVector2& windowRect) const override;

	virtual void Update(float deltaTime) override;

	void SetRemainingTime(float time) { m_RemainingTime = time; }

	void SetMaxTime(float time) { m_MaxTime = time; }

	void SetScore(const tstring& score) { m_pCurrentScore->SetText(score); }

	void SetHiscore(const tstring& score) { m_pHiScoreValue->SetText(score); }

	void ResetTimer() { m_RemainingTime = m_MaxTime; }

	void LoseLife();

private:

	void ReachGoal();

	void WinGame();

public:

	IDelegate<> OnGameOver;

	IDelegate<> OnGameWin;

private:

	Text* m_p1Up{};
	Text* m_pHiScore{};
	Text* m_pCurrentScore{};
	Text* m_pHiScoreValue{};
	Text* m_pTimeText{};

	std::vector<Texture*> m_LiveTextures;

	Texture* m_pFrogGoals[5]{};

	class AudioComponent* m_pMusic{};

	Bitmap m_ResourceTexture;

	float m_RemainingTime{};
	float m_MaxTime{};

	int m_Lives{ 6 };

	bool m_bPauseTimer{};

};

