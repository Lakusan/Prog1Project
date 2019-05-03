#include "raylib.h"
#include <iostream>





typedef struct {
	int sizex = GetScreenWidth() / 32;
		int sizey = GetScreenHeight() / 18;
	bool acitve = true;
}box;

typedef struct {
	int score = 0;
	float posx = 0;
	float posy = 0;
	Vector2 vPos = { 0,0 };
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
	Rectangle Boundarys = { 0,0,(float)(GetScreenWidth()),(float)(GetScreenHeight()) };
	//init PlayerPos
	player1.posx = 2 * (GetScreenWidth() / 32);
	player1.posy = 2 * (GetScreenHeight() / 18);
	player1.vPos = { player1.posx, player1.posy };

	///*Rectangle Boundarys_edges = { 120,120,(float)(GetScreenWidth()),(float)(GetScreenHeight()) };*/ 
	//Hier switch für levelauswahl


	Rectangle PlayerHitBox = { player1.posx,player1.posy,(GetScreenWidth()/32),(GetScreenHeight()/18) };

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		//GRID 
		


		//init map
		




		//Input Keyboard

		if (IsKeyDown(KEY_RIGHT))
		{
			if (player1.posx < ((GetScreenWidth()) - (2 * (GetScreenWidth() / 32))))
			{
				/*	std::cout << player1.posx << std::endl;
					std::cout << player1.posy << std::endl;*/


				player1.posx += player1.speed;
				PlayerHitBox.x = player1.posx;
			}
		}
		if (IsKeyDown(KEY_LEFT))
		{
			if (player1.posx > (GetScreenWidth() / 32))
			{
				player1.posx -= player1.speed;
				PlayerHitBox.x = player1.posx;
			}
		}
		if (IsKeyDown(KEY_UP))
		{
			if (player1.posy > (GetScreenHeight() / 18))
			{
				player1.posy -= player1.speed;
				PlayerHitBox.y = player1.posy;
			}
		}
		if (IsKeyDown(KEY_DOWN))
		{
			if (player1.posy < (GetScreenHeight()) - (2 * (GetScreenHeight() / 18)))
			{
				player1.posy += player1.speed;
				PlayerHitBox.y = player1.posy;
			}
		}

		//Input Mouse

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			
				player1.posx = GetMouseX();
				player1.posy = GetMouseY();
				PlayerHitBox.x = player1.posx;
				PlayerHitBox.y = player1.posy;
			
		}


		//Collisiondetection protootype
		/*if (!CheckCollisionRecs(PlayerHitBox, Boundarys_edges))
		{
			for (int i = 0; i < 10; i++) {
				std::cout << "HIT" << i << std::endl;
			}
		}*/





		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);
		//GameLoop
		//Draw Level Boundarys and PlayerHitBox
		for (int i = 0; i <= 32; i++)
		{
			//Boundary
			DrawRectangleLinesEx(Boundarys, (GetScreenHeight() / 18), RED);
			//PlayerHitBox
			DrawRectangleLinesEx(PlayerHitBox, 10, BLUE);

		}
		// Draw Development Grid
		for (int i = 1; i <= 32; i++)
		{
			DrawLine(i*(GetScreenWidth() / 32), GetScreenHeight()/18, i*(GetScreenWidth() / 32), (GetScreenHeight()-(GetScreenHeight()/18)), BLACK);
			DrawLine((GetScreenWidth() / 32), i*(GetScreenHeight()/18),  (GetScreenWidth())-(GetScreenWidth()/32), i*(GetScreenHeight()/18), BLACK);

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