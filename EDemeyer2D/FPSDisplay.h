#pragma once
#include "IObject.h"

class FPSDisplay final : private IObject
{
public:

    FPSDisplay();
    virtual ~FPSDisplay();

    virtual void Update(float deltaTime) override;
    virtual void Draw(const FVector2& windowSize) const override;

protected:

    class Text* m_pText;
    double m_AccumulatedTime{};
};

