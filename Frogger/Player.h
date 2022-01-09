#pragma once
#include "Frog.h"
#include "IInputReceiver.h"
#include "Delegates.h"

class Texture;
class Sprite;
class BoxCollision;

class Player final :
    public Frog
{
public:

    Player();
    virtual ~Player() = default;

};

