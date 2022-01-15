#pragma once
#include "IComponent.h"
#include "Delegates.h"

class TimeLineComponent :
    public IComponent
{
public:

    TimeLineComponent() = default;
    virtual ~TimeLineComponent() = default;

    virtual void Update(float deltaTime) override;

    void SetCurrentTimeStamp(float timeStamp) { m_CurrentTime = timeStamp; }

    float GetCurrentTimeStamp() const { return m_CurrentTime; }

    void SetLooping(bool value) { m_bIsLooping = value; }

    void SetPaused(bool value) { m_bIsPaused = value; }

    void Play() { m_bIsPaused = false; m_CurrentTime = 0; }

    IDelegate<> OnEnd;

    IDelegate<float> OnTick;

protected:

    float m_CurrentTime{};

    float m_MaxTime{};

    bool m_bIsPaused{};
    bool m_bIsLooping{};

};

