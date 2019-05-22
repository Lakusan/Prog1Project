#pragma once


class Player {
public:
	int score;
	int gold;
	//position player
	float posx;
	float posy;
	Vector2 vPos;
	Vector2 vDirectionVector;
	Rectangle vDirection;
	int directionState;
	bool collisionVDirection;
	bool shield;
	bool blocked;

	
	
	int lifes;
	float speed;
	//init starting point per level
	float startx;
	float starty;
	//PlayerHitBox
	Rectangle PlayerHitBox;
	//methods
	Player();
	~Player();
	void SetStartingPosx();
	float GetStartingPosx();
	void SetStartingPosy();
	float GetStartingPosy();
	void SetDirectionState();
};