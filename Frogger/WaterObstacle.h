#pragma once
#include <IObject.h>

class WaterObstacle :
    public IObject
{
public:
    WaterObstacle();
    virtual ~WaterObstacle() = default;

public:

    virtual void Update(float deltaTime) override;

    void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

    virtual void OnOverlapObject(IObject* other, ICollisionComponent* component = nullptr, ICollisionComponent* otherComponent = nullptr);

protected:

    float m_MovementSpeed{};

    class BoxCollision* m_pCollision{};

    static FVector2 HorizontalBounds;
};

