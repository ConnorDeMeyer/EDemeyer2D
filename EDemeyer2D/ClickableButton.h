#pragma once
#include "IObject.h"
#include "Delegates.h"
#include "Text.h"

class ClickableButton :
    public IObject
{
public:

    ClickableButton();
    virtual ~ClickableButton();

    IDelegate<> OnClick;

    virtual void Draw(const FVector2& windowSize) const override;

    void SetDimensions(float width, float height);

    void SetButtonColor(const FColor& color) { m_Color = color; }
    void SetTextColor(const FColor& color) { m_pText->SetColor(color); }

    void SetText(const tstring& text) { m_pText->SetText(text); }
    void SetFont(const tstring& font) { m_pText->SetFont(font); }

    void Click(int x, int y);

private:

    FColor m_Color{ 0,0.75f,1,1 };

    Text* m_pText{};

    class BoxCollision* m_pCollision{};

};

