#include "Scene_TitleScreen.h"
#include "ResourceManager.h"
#include "DrawingManager.h"
#include "Frog.h"

Scene_TitleScreen::Scene_TitleScreen()
{
	Frog* pFrog{ new Frog() };
	pFrog->SetPosition({ 20,20 });
	AddObjectToScene(pFrog);
}

Scene_TitleScreen::~Scene_TitleScreen()
{
}

void Scene_TitleScreen::Draw(const FVector2& windowSize) const
{
	DRAW->FillRectangle(FRect{ 0,0,windowSize.x,windowSize.y * 0.5f }, FColor{0,0,0.278f,1});
	DRAW->FillRectangle(FRect{ 0,windowSize.y * 0.5f,windowSize.x,windowSize.y * 0.5f }, FColor{ 0,0,0,1 });
	
	Scene::Draw(windowSize);
}
