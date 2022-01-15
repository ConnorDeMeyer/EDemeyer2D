#include "TimeLineComponent.h"

void TimeLineComponent::Update(float deltaTime)
{
	IComponent::Update(deltaTime);

	if (m_bIsPaused) return;

	OnTick.BroadCast(m_CurrentTime);

	m_CurrentTime += m_MaxTime;

	if (m_CurrentTime >= m_MaxTime)
	{
		m_bIsPaused = !m_bIsLooping;
		m_CurrentTime = 0;
		OnEnd.BroadCast();
	}
}
