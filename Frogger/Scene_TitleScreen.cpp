#include "Scene_TitleScreen.h"
#include "ResourceManager.h"
#include "DrawingManager.h"
#include "Frog.h"
#include "Car.h"
#include "Player.h"

Scene_TitleScreen::Scene_TitleScreen()
{
	Frog* pFrog{ new Player() };
	pFrog->SetPosition({ 100,130 });
	AddObjectToScene(pFrog);

	Car* pCar{ new Car() };
	pCar->SetPosition({ 100,100 });
	pCar->SetMovementSpeed(-10.f);
	AddObjectToScene(pCar);

	pCar = new Car();
	pCar->SetPosition({ 130,100 });
	pCar->SetMovementSpeed(-10.f);
	AddObjectToScene(pCar);
}

void Scene_TitleScreen::Draw(const FVector2& windowSize) const
{
	DRAW->FillRectangle(FRect{ 0,0,windowSize.x,windowSize.y * 0.5f }, FColor{0,0,0.278f,1});
	DRAW->FillRectangle(FRect{ 0,windowSize.y * 0.5f,windowSize.x,windowSize.y * 0.5f }, FColor{ 0,0,0,1 });
	
	Scene::Draw(windowSize);
}
