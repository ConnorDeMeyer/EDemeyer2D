#pragma once
#include <vector>

class Scene;

class GameInstance
{
	friend class GameEngine;

public:

	GameInstance();
	virtual ~GameInstance();

	GameInstance(const GameInstance& other) = delete;
	GameInstance(GameInstance&& other) = delete;
	GameInstance& operator=(const GameInstance& other) = delete;
	GameInstance& operator=(GameInstance&& other) = delete;

	Scene* GetCurrentScene() const { return m_pCurrentScene; }
	void SetCurrentScene(Scene* pScene);

	virtual void Update(float deltaTime) {};

	void DestroyFlaggedScenes();

private:

	Scene* m_pCurrentScene{};

	std::vector<Scene*> m_LoadedScenes;

};

