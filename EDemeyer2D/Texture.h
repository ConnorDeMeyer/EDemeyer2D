#pragma once
#include "IComponent.h"
#include "Bitmap.h"

class Texture :
    public IComponent
{
public:

    Texture() = default;
    Texture(const Bitmap& bitmap) : m_Texture{ bitmap } {}
    virtual ~Texture() = default;

    virtual void Draw(const FVector2& windowSize) const override;

    //will also reset source rect
    void SetTexture(Bitmap& bitmap);

    void SetSourceRect(const FRect& srcRect) { m_SourceRect = srcRect; }

protected:

    Bitmap m_Texture;

    FRect m_SourceRect = {};

};

