#pragma once
#include "Scene.h"

class Scene_TitleScreen final :
    public Scene
{
public:

    Scene_TitleScreen();
    virtual ~Scene_TitleScreen() = default;


    virtual void Draw(const FVector2& windowSize) const override;

private:


};

