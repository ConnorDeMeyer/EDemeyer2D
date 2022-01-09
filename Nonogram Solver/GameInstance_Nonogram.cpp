#include "GameInstance_Nonogram.h"
#include "GameEngine.h"
#include "Scene_NonogramVisual.h"

GameInstance_Nonogram::GameInstance_Nonogram()
{

	ENGINE->SetAutomaticResolutionChange(true);
	ENGINE->SetWindowSize({ 1280,720 });

	SetCurrentScene(new Scene_NonogramVisual());
}

GameInstance_Nonogram::~GameInstance_Nonogram()
{

}
