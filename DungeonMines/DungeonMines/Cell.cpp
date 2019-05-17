#include "raylib.h"
#include "cell.h"

//Constructor
Cell::Cell()
{
	SetCellWidth();
	SetCellHeight();
	SetFirstCellPosX();
	SetFirstCellPosY();
	GetCellPosX();
	GetCellPosY();
	
	posx = 0.0f;
	posy = 0.0f;
	width = GetCellWidth();
	height = GetCellHeight();
	vPos = { GetCellPosX(),GetCellPosY() };
	CellHitBox = { GetCellPosX(),GetCellPosY() ,60,60 };
	destructible = true;
	active = true;
	hasBomb = false;
	revealed = false;
	isShield = false;
	isExit = false;
	isGold = false;
	isTrap = false;
	state = 0;
	hint = 0;
	animationCounter = 0;

	firstCellPosX = GetFirstCellPosX();
	firstCellPosY = GetFirstCellPosY();
}

//Destructor
Cell::~Cell()
{

}

// Setter
void Cell::SetCellWidth()
{
	this->width = (GetScreenWidth() / 32);
}

void Cell::SetCellHeight()
{
	this->height = (GetScreenHeight() / 18);
}

void Cell::SetFirstCellPosX()
{
	this->firstCellPosX = GetCellWidth();
}
void Cell::SetFirstCellPosY()
{
	this->firstCellPosY = GetCellHeight();
}

// Getter
int Cell::GetCellState()
{
	return this->state;
}

float Cell::GetCellWidth()
{
	return this->width;
}

float Cell::GetCellHeight()
{
	return this->height;
}

float Cell::GetFirstCellPosX()
{
	return this->firstCellPosX;
}

float Cell::GetFirstCellPosY()
{
	return this->firstCellPosY;
}

float Cell::GetCellPosX()
{
	return this->posx;
}

float Cell::GetCellPosY()
{
	return this->posy;
}

Vector2 Cell::GetCellVector()
{
	Vector2 Vector = { posx,posy };
	return Vector;
}

