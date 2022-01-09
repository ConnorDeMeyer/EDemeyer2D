#pragma once

#include "Scene_Level.h"
#include <vector>
#include <fstream>
#include <sstream>
#include "Car.h"
#include "Turtle.h"
#include "LogSegment.h"
#include "BoxCollision.h"

void LoadLevel(const std::string& levelString, int levelNr, Scene_Level* pLevel);

void LoadLevel(int level, Scene_Level* pLevel)
{
	if (!pLevel) return;

	auto file = std::ifstream("Resources/Saves/LevelLayout.lvl");
	while (file)
	{
		unsigned char firstChar{};
		file >> firstChar;

		switch (firstChar)
		{
		case '#':
			break;
		case 'l':
			int fLevel{};
			file >> fLevel;
			if (fLevel == (level%5))
			{
				std::string levelString;
				std::getline(file, levelString);
				LoadLevel(levelString, level, pLevel);
				file.close();
				return;
			}
			break;
		}
		file.ignore(MAXINT, '\n');
	}

	file.close();
}

void LoadLevel(const std::string& levelString, int levelNr, Scene_Level* pLevel)
{
	std::stringstream levelStream(levelString);

	//const float LevelWidth{ 304 };
	const float LevelWidth{ 288 };
	char info[3];

	// first row
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i)
		{
			Car* pCar{ new Car() };
			pCar->SetCarType(eCarType::CarLeft2);
			pCar->SetPosition(FVector2{ float(i * LevelWidth / amount - 32), float(216) });
			pCar->SetMovementSpeed(-10.f);
			pLevel->AddObjectToScene(pCar);
		}
	}

	// second row
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i)
		{
			Car* pCar{ new Car() };
			pCar->SetCarType(eCarType::CarRight2);
			pCar->SetPosition(FVector2{ float(i * LevelWidth / amount - 32), float(200) });
			pCar->SetMovementSpeed(8.f);
			pLevel->AddObjectToScene(pCar);
		}
	}

	// third row
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i)
		{
			Car* pCar{ new Car() };
			pCar->SetCarType(eCarType::CarLeft1);
			pCar->SetPosition(FVector2{ float(i * LevelWidth / amount - 32), float(184) });
			pCar->SetMovementSpeed(-16.f);
			pLevel->AddObjectToScene(pCar);
		}
	}

	// fourth row
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i)
		{
			Car* pCar{ new Car() };
			pCar->SetCarType(eCarType::CarRight1);
			pCar->SetPosition(FVector2{ float(i * LevelWidth / amount - 32), float(168) });
			pCar->SetMovementSpeed(32.f);
			pLevel->AddObjectToScene(pCar);
		}
	}

	// fifth row
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i)
		{
			Car* pCar{ new Car() };
			pCar->SetCarType(eCarType::CarLeftBig);
			pCar->SetPosition(FVector2{ float(i * LevelWidth / amount - 32), float(152) });
			if (BoxCollision* col = pCar->GetComponent<BoxCollision>()) col->SetBoundingRect(FRect{ -14, -6,13,7 });
			pCar->SetMovementSpeed(-8.f);
			pLevel->AddObjectToScene(pCar);
		}
	}

	// Water row 1
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i) {
			for (int j{}; j < 3; ++j)
			{
				Turtle* pTurtle{ new Turtle() };
				pTurtle->SetPosition(FVector2{ float(i * 256 / amount + j * 16.f), float(120) });
				pTurtle->SetMovementSpeed(-8.f);
				pLevel->AddObjectToScene(pTurtle);
			}
		}
	}

	// Water row 2
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i) {
			for (int j{}; j < 3; ++j)
			{
				LogSegment* pLog = new LogSegment();
				pLog->SetPosition(FVector2{ float(i * 256 / amount + j * 16.f), float(104) });
				if (j == 0) pLog->SetLogSegment(eLogSegment::logEndLeft);
				else if (j == 2) pLog->SetLogSegment(eLogSegment::logEndRight);
				pLog->SetMovementSpeed(6);
				pLevel->AddObjectToScene(pLog);
			}
		}
	}

	// Water row 3
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i) {
			for (int j{}; j < 6; ++j)
			{
				LogSegment* pLog = new LogSegment();
				pLog->SetPosition(FVector2{ float(i * 256 / amount + j * 16.f), float(88) });
				if (j == 0) pLog->SetLogSegment(eLogSegment::logEndLeft);
				else if (j == 5) pLog->SetLogSegment(eLogSegment::logEndRight);
				pLog->SetMovementSpeed(12);
				pLevel->AddObjectToScene(pLog);
			}
		}
	}

	// Water row 4
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i) {
			for (int j{}; j < 3; ++j)
			{
				Turtle* pTurtle{ new Turtle() };
				pTurtle->SetPosition(FVector2{ float(i * 256 / amount + j * 16.f), float(72) });
				pTurtle->SetMovementSpeed(-8.f);
				pLevel->AddObjectToScene(pTurtle);
			}
		}
	}

	// Water row 5
	{
		levelStream >> info;
		int amount{ int(info[1] - '0') };
		for (int i{}; i < amount; ++i) {
			for (int j{}; j < 4; ++j)
			{
				LogSegment* pLog = new LogSegment();
				pLog->SetPosition(FVector2{ float(i * 256 / amount + j * 16.f), float(56) });
				if (j == 0) pLog->SetLogSegment(eLogSegment::logEndLeft);
				else if (j == 3) pLog->SetLogSegment(eLogSegment::logEndRight);
				pLog->SetMovementSpeed(4);
				pLevel->AddObjectToScene(pLog);
			}
		}
	}
}

