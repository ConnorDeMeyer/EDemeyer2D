#pragma once
#include "Scene.h"
#include "Nonogram.h"
#include "DrawingManager.h"
#include <future>
#include <functional>

class Scene_NonogramVisual : public Scene
{
public:

	Scene_NonogramVisual();
	virtual ~Scene_NonogramVisual();

	virtual void Update(float deltaTime) override;
	
	virtual void Draw(const FVector2& windowSize) const override;

	// Transfers owner ship to scene and make it appear on the board
	void SetNonogram(Nonogram* pNonogram);

	Nonogram* GetNonogram() { return m_pNonogram; }

	void StartSolvingThread(std::function<void()> pSolver);

private:

	void ClickMouse(int x, int y);

	void UpdateTextSize(float size);
	void UpdateHintSize();

	Nonogram* m_pNonogram{};
	int m_VerticalHintSize{};
	int m_HorizontalHintSize{};

	FRect m_GridBounds{};
	float m_SquareSize{};
	FVector2 m_GridOffset{};

	IDWriteFactory* m_pWriteFactory{};
	IDWriteTextFormat* m_pTextFormat{};

	std::vector<class ClickableButton*> m_Buttons;
	ClickableButton* m_CancelButton{};

	std::future<void> m_SolvingThread;
};

