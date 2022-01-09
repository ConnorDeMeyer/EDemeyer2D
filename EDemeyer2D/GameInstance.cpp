#include "GameInstance.h"
#include "Scene.h"


GameInstance::GameInstance()
{
}

GameInstance::~GameInstance()
{
    delete m_pCurrentScene;
}

void GameInstance::SetCurrentScene(Scene* pScene)
{
    if (m_pCurrentScene) delete m_pCurrentScene;
    m_pCurrentScene = pScene;
}

void GameInstance::DestroyFlaggedScenes()
{
    if (m_pCurrentScene->GetDestroyFlag())
    {
        delete m_pCurrentScene;
        m_pCurrentScene = nullptr;
    }
}
