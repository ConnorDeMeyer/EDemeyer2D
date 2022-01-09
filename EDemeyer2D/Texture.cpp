#include "Texture.h"
#include "DrawingManager.h"

void Texture::Draw(const FVector2& windowSize) const
{
	IComponent::Draw(windowSize);

	FVector2 pos{ GetPosition() };
	FVector2 scale{ GetScale() };
	FVector2 drawingDim{ m_SourceRect.right - m_SourceRect.left, m_SourceRect.bottom - m_SourceRect.top };
	FRect drawRect{
		pos.x - scale.x * drawingDim.x * 0.5f,
		pos.y - scale.y * drawingDim.y * 0.5f,
		pos.x + scale.x * drawingDim.x * 0.5f,
		pos.y + scale.y * drawingDim.y * 0.5f
	};

	DRAW->DrawBitmap(m_Texture, drawRect, m_SourceRect);
}

void Texture::SetTexture(Bitmap& bitmap)
{
	m_Texture = bitmap;
	m_SourceRect = FRect{ 0,0,bitmap.GetSize().width, bitmap.GetSize().height };
}
