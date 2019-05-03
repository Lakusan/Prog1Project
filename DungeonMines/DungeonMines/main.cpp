#include "raylib.h"
#include <iostream>


typedef struct {
	int sizex = GetScreenWidth() / 32;
	int sizey = GetScreenHeight() / 18;
	bool acitve = true;
}box;

typedef struct {
	int score=0;
	int posx=220;
	int posy=220;
	Vector2 vPos = { 220,220 };
	int speed = 5;
}Player;

Player player1;


int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	//init Screen
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	ToggleFullscreen();
	InitWindow(screenWidth, screenHeight, "INTRO");
	SetTargetFPS(60);

	//init levels
	Rectangle Boundarys = { 0,0,(float)(GetScreenWidth()),(float)(GetScreenHeight())};
	Rectangle Boundarys_innen = { 120,120,(float)(GetScreenWidth()),(float)(GetScreenHeight()) };

	Rectangle PlayerHitBox = {220,220,120,120};

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		//Input Keyboard

		if (IsKeyDown(KEY_RIGHT))
		{
			if(player1.posx <= ((GetScreenWidth()) - (2*(GetScreenWidth() / 32))))
			{
				std::cout << player1.posx << std::endl;
				std::cout << player1.posy << std::endl;
			

				player1.posx += player1.speed;
				PlayerHitBox.x = player1.posx;
			}
		}
		if (IsKeyDown(KEY_LEFT))
		{
			player1.posx -= player1.speed;
			PlayerHitBox.x = player1.posx;
		}
		if (IsKeyDown(KEY_UP))
		{
			player1.posy -= player1.speed;
			PlayerHitBox.y = player1.posy;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			player1.posy += player1.speed;
			PlayerHitBox.y = player1.posy;
		}

		//Input Mouse

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			player1.posx = GetMouseX();
			player1.posy = GetMouseY();
			PlayerHitBox.x = player1.posx;
			PlayerHitBox.y = player1.posy;
		}


		//Collisiondetection
		if (!CheckCollisionRecs(PlayerHitBox,Boundarys_innen))
		{
			for (int i = 0; i < 10; i++) {
				std::cout << "HIT"<<i<< std::endl;
			}
		}





		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);
		//GameLoop
		DrawGrid(5, 100);
		for (int i = 0; i <= 32; i++)
		{
			

				DrawRectangleLinesEx(Boundarys,(GetScreenHeight()/18), RED);
				DrawRectangleLinesEx(PlayerHitBox, 10,BLUE);




			

		}
		EndDrawing();
		//----------------------------------------------------------------------------------

	}
			// De-Initialization
			//--------------------------------------------------------------------------------------   
		CloseWindow();        // Close window and OpenGL context
		//--------------------------------------------------------------------------------------

		return 0;

}