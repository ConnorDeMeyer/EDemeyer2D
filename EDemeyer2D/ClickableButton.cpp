#include "ClickableButton.h"
#include "InputManager.h"
#include "DrawingManager.h"
#include "Text.h"
#include "Collision.h"
#include "BoxCollision.h"
#include "Delegates.h"

using namespace std::placeholders;

ClickableButton::ClickableButton()
{
	INPUT->BindMouseUpAction(InputKeys::MouseLeft, this, std::bind(&ClickableButton::Click, this, _1, _2));

	m_pText = new Text();
	m_pText->SetText(_T("TEST"));
	m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	AddComponent(m_pText);

	m_pCollision = new BoxCollision();
	AddComponent(m_pCollision);
}

ClickableButton::~ClickableButton()
{
}

void ClickableButton::Draw(const FVector2& windowSize) const
{
	DRAW->FillRoundedRectangle(m_pCollision->GetWorldBoundingRect(), min(m_pCollision->GetWidth(), m_pCollision->GetHeight()) * 0.125f, m_Color);

	IObject::Draw(windowSize);
}

void ClickableButton::SetDimensions(float width, float height)
{
	m_pCollision->SetBoundingRect({ -width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f });
	m_pText->SetTextSize(height * 0.5f);
	m_pText->SetDrawingWidth(width);
}

void ClickableButton::Click(int x, int y)
{
	if (IsOverlapping(FPoint2{ float(x),float(y) }, m_pCollision->GetWorldBoundingRect()))
		OnClick.BroadCast();
}
