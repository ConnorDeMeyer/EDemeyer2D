#include "FPSDisplay.h"
#include "Text.h"
#include <tchar.h>

FPSDisplay::FPSDisplay()
{
	m_pText = new Text(_T("0.00"), _T("Resources/Fonts/advanced_pixel-7.ttf"), 16.f);
	m_pText->SetPosition(FVector2(0, 0));
	m_pText->SetColor(FColor{ 1,1,0,1 });
	AddComponent(m_pText);
}

FPSDisplay::~FPSDisplay()
{
	m_pText->Destroy(); // not neccesary as engine will delete this automatically
}

void FPSDisplay::Update(float deltaTime)
{
	IObject::Update(deltaTime);

	m_AccumulatedTime += deltaTime;
	if (m_AccumulatedTime > 0.2f) // only update 5 times a second
	{
		m_pText->SetText(std::to_wstring(1.0 / deltaTime));
		m_AccumulatedTime = 0;
	}
}

void FPSDisplay::Draw(const FVector2& windowSize) const
{
	IObject::Draw(windowSize);
}
