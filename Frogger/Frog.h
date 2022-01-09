#pragma once
#include <IObject.h>
#include "Bitmap.h"
#include "Delegates.h"

class Texture;
class Sprite;
class BoxCollision;

enum class eMovementDirection : UINT8{
    up,
    right,
    down,
    left
};

enum class eFrogType : UINT8
{
    Green = 0,
    Pink = 1
};

class Frog :
    public IObject
{
public:

    Frog();
    virtual ~Frog() = default;

    virtual void Update(float deltaTime) override;

    void MoveDirection(eMovementDirection direction);

    void SetWorldBounds(const FRect& bounds) { m_WorldBounds = bounds; }

    void Die();

    void Respawn();

    void SetTextureDirection(eMovementDirection direction, bool isLeaping = false);

    bool IsLeaping() { return m_bLeaping; }

    void SetFrogType(eFrogType type) { m_FrogType = type; }

    IDelegate<> OnFrogDeath;

private:

    void MoveToClosestRow();

    Texture* m_pFrogTexture = {};
    Sprite* m_pDeathSprite = {};
    BoxCollision* m_pBoxCollision = {};

    float m_MoveCouldown = {};

    FVector2 m_Movement = {};
    float m_MovementSpeed = { 48.f };
    FRect m_WorldBounds = { 0,2.f * 16.f,14.f * 16.f,15.f * 16.f };

    FVector2 m_SpawnPos{ 112, 232 };

    bool m_bCanMove = { true };
    bool m_bImmune = { false };
    bool m_bSnapToTile = { true };
    bool m_bLeaping = { false };

    eMovementDirection m_CurrentDirection = { eMovementDirection::up };
    eFrogType m_FrogType = { eFrogType::Green };
};