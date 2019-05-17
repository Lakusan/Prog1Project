#include "raylib.h"
#include "player.h"

Player::Player() {
	SetStartingPosx();
	SetStartingPosy();
	//Player Score
	score = 0;
	//money
	gold = 0;
	//position player
	posx = GetStartingPosx();
	posy = GetStartingPosy();
	vPos = { posx,posy };
	vDirection = {(posx+30.0f),(posy+((GetScreenHeight()/18.0f)+30.0f))};
	vDirectionVector = { Player::vDirection.x,Player::vDirection.y };
	//player attributes for powerups punishments and game over
	
	lifes = 3;
	speed = 60.0f;
	//init starting point per level
	startx = 0.0f;
	starty = 0.0f;
	PlayerHitBox = { posx,posy,(float)((GetScreenWidth())/ 32),(float)((GetScreenHeight()) / 18) };
	directionState = 0;
	collisionVDirection = false;
	shield = false;
	blocked = false;

}

Player::~Player()
{

}

void Player::SetStartingPosx()
{
	startx = ((GetRandomValue(2, 29)) * (GetScreenWidth() / 32));	
}

float Player::GetStartingPosx()
{
	return startx;
}


void Player::SetStartingPosy()
{
	starty = ((GetRandomValue(2, 15)) * (GetScreenHeight() / 18));
}

float Player::GetStartingPosy()
{
	return starty;
}

void Player::SetDirectionState()
{
	//Player Viewing direction down
	if (vDirection.x == (posx+30)&& vDirection.y > (posy+90))
		directionState = 0;
	//Player Viewing direction up
	if (vDirection.x == (posx+30) && vDirection.y < (posy-30))
		directionState = 1;
	//Player Viewing direction right
	if (Player::vDirection.x > (Player::posx+90) && Player::vDirection.y == (Player::posy+30))
		Player::directionState = 2;
	//Player Viewing direction left
	if (Player::vDirection.x < (Player::posx-30) && Player::vDirection.y == (Player::posy+30))
		Player::directionState = 3;
}
