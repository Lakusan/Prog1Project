#pragma once

//Class initializes all Cells for GameWorld

class Cell {

public:
	//Constructor 
		Cell();
	//destructor
		~Cell();
	//methods
		//setter
		void SetCellWidth();
		void SetCellHeight();
		void SetFirstCellPosX();
		void SetFirstCellPosY();

		//getter
		int GetCellState();
		float GetCellWidth();
		float GetCellHeight();
		float GetFirstCellPosX();
		float GetFirstCellPosY();
		float GetCellPosX();
		float GetCellPosY();
		Vector2 GetCellVector();


	//position cell
		float posx;
		float posy;
		float width;
		float height;
		Vector2 vPos;
	//Vector2 vPos = { 0,0 };
	//CollisionDetection
		Rectangle CellHitBox;
	//cell logic
		bool destructible;
		bool active;
		bool hasBomb;
		bool revealed;
		bool isShield;
		bool isExit;
		bool isGold;
		bool isTrap;
		int state;
		int hint;
		int animationCounter;


	//init values
		float firstCellPosX;
		float firstCellPosY;


};
