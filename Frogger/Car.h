#pragma once
#include <IObject.h>
#include "Texture.h"
#include "Structs.h"

class BoxCollision;

enum class eCarType : UINT8
{
    CarLeft1,
    CarLeft2,
    CarRight1,
    CarRight2,
    CarLeftBig
};

class Car final :
    public IObject
{
public:

    Car();
    virtual ~Car() = default;

    virtual void Update(float deltaTime) override;

    virtual void OnOverlapObject(IObject* other, ICollisionComponent* component = nullptr, ICollisionComponent* otherComponent = nullptr) override;

    virtual void OnBeginOverlap(IObject* other) override;

    void SetCarType(eCarType carType);

    void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

    static void SetHorizontalBounds(const FVector2& bounds) { HorizontalBounds = bounds; }

private:

    Texture* m_pTexture{};

    BoxCollision* m_Collider{};

    float m_MovementSpeed{};

    static FVector2 HorizontalBounds;

};

