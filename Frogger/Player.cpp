#include "Player.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"
#include "BoxCollision.h"
#include "GameInstance_Frogger.h"
#include "Collision.h"

Player::Player()
{
	INPUT->BindKeyPressedAction(InputKeys::RightArrow, this, std::bind(&Player::MoveDirection, this, eMovementDirection::right));
	INPUT->BindKeyPressedAction(InputKeys::LeftArrow, this, std::bind(&Player::MoveDirection, this, eMovementDirection::left));
	INPUT->BindKeyPressedAction(InputKeys::UpArrow, this, std::bind(&Player::MoveDirection, this, eMovementDirection::up));
	INPUT->BindKeyPressedAction(InputKeys::DownArrow, this, std::bind(&Player::MoveDirection, this, eMovementDirection::down));
}
