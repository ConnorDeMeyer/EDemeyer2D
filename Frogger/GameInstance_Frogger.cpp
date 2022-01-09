#include "GameInstance_Frogger.h"
#include "ResourceManager.h"
#include "Scene_TitleScreen.h"
#include "Scene_Level.h"
#include "framework.h"

GameInstance_Frogger::GameInstance_Frogger()
{
	// Permanently load the frog, enemy, obstacle and ground textures
	RESOURCES->PermanentlyLoadBitmap(_T("Resources/Bitmaps/Frogger_Death.png"));
	RESOURCES->PermanentlyLoadBitmap(_T("Resources/Bitmaps/Frogger_Enemies.png"));
	RESOURCES->PermanentlyLoadBitmap(_T("Resources/Bitmaps/Frogger_Frogs.png"));
	RESOURCES->PermanentlyLoadBitmap(_T("Resources/Bitmaps/Frogger_Misc.png"));
	RESOURCES->PermanentlyLoadBitmap(_T("Resources/Bitmaps/Frogger_Obstacles.png"));

	SetCurrentScene(new Scene_Level());

	ENGINE->SetWindowResolution(224, 256);
	ENGINE->SetWindowSize(3 * ENGINE->GetWindowResolution());

	DRAW->BeginDraw();
	DRAW->FillRectangle({ 64,64,100,100 }, { 0,1,0,1 });
	DRAW->EndDraw();
}

GameInstance_Frogger::~GameInstance_Frogger()
{
}
