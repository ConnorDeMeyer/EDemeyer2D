#include "Sprite.h"
#include "DrawingManager.h"

void Sprite::Draw(const FVector2& windowSize) const
{
	IComponent::Draw(windowSize);

	FVector2 pos{ GetPosition() };
	FVector2 scale{ GetScale() };
	FRect destRect{
		pos.x - m_FrameDimension.x * scale.x * 0.5f, pos.y - m_FrameDimension.y * scale.y * 0.5f,
		pos.x + m_FrameDimension.x * scale.x * 0.5f, pos.y + m_FrameDimension.y * scale.y * 0.5f };
	FRect sourceRect{
		m_CurrentFrameX * m_FrameDimension.x,
		m_CurrentFrameY * m_FrameDimension.y,
		(m_CurrentFrameX + 1) * m_FrameDimension.x,
		(m_CurrentFrameY + 1) * m_FrameDimension.y };
	
	DRAW->DrawBitmap(m_Bitmap, destRect, sourceRect);
}

void Sprite::Update(float deltaTime)
{
	IComponent::Update(deltaTime);

	if (m_bPauseTime) return;

	m_AccumulatedTime += deltaTime;
	if (m_AccumulatedTime > m_TimePerFrame)
	{
		m_AccumulatedTime -= m_TimePerFrame;
		if (++m_CurrentFrame >= m_TotalFrames + m_FrameOffset)
		{
			if (m_bLoop)
				m_CurrentFrame = m_FrameOffset;
			else
				m_bPauseTime = true;

			OnAnimationEnd.BroadCast();
		}

		int horizontalFrames{ int(m_Bitmap.GetSize().width / m_FrameDimension.x) };
		m_CurrentFrameY = m_CurrentFrame / horizontalFrames;
		m_CurrentFrameX = m_CurrentFrame % horizontalFrames;
	}
}

void Sprite::SetCurrentFrame(int frame)
{
	m_CurrentFrame = frame;
	int horizontalFrames{ int(m_Bitmap.GetSize().width / m_FrameDimension.x) };
	m_CurrentFrameY = m_CurrentFrame / horizontalFrames;
	m_CurrentFrameX = m_CurrentFrame % horizontalFrames;
}

void Sprite::Reset()
{
	m_CurrentFrame = m_FrameOffset;
	m_AccumulatedTime = 0;
	m_bPauseTime = false;
}
