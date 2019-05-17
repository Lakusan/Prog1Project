#include "raylib.h"
#include <iostream>
//Header files
#include "Cell.h"
#include "player.h"
#include "textures.h"
//represents the level
#define COLUMNS 30
#define ROWS 16 
//animation settings
#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1





//ScreenSelection
typedef enum GameScreen { TITLE, MENU, GAMEPLAY, CREDITS, GAMEOVER } GameScreen;
GameScreen currentScreen = TITLE;
int currentSelectedOption = 0;
int currentSelectedSetting = 0;
bool controllerActive = false;
bool languageGerman = false;

//forward declarations for funktions
void initScreen();
void initPlayer();
void initCells();
float getCurrentScreenWidthAsFloat();
float getCurrentScreenHeightAsFloat();
//frames
float framewidth = (float)(GetScreenWidth() / 32);
//amount of pictures per tileset
int maxFrames = 4;
//timer for animation speed
float frameTimer = 0.0f;
int currentFrame = 0;

//holds active Level
int currentLevel = 1;

//holds active difficulty modifier
//Difficulty Settings as modificator for increasing difficulty durig progress
//no Easy, because easy settings are the defaults
int easy = 10;
int medium = 5;
int hard = 1;
int currentDifficulty = 5;

//ingame object quantity
int maxFieldsPerLevel = COLUMNS * ROWS;

//Create objects
Player player1;
Cell board[30][16];


//create Textureobjects
Textures layer0Textures[COLUMNS][ROWS];//floor
Textures layer1Textures[COLUMNS][ROWS];//Objects on floor 
Textures layer3Textures[COLUMNS][ROWS];//Fog of War
Texture2D Monster;
Texture2D cloud[3];
Texture2D Rock;
Texture2D Hero;
Texture2D PowerUp[2];
Texture2D LevelBackGround;
Texture2D Health[3];
//Texture2D Light;
Texture2D MediPack;
Texture2D Sparkles;
Texture2D ShieldIndicator;
Texture2D Shield;
Texture2D Light_right;
Texture2D Light_left;
Texture2D Light_up;
Texture2D Light_down;
Texture2D Ladder;
Texture2D Gold;
Texture2D Trap;
Texture2D MonsterDespawn;
Texture2D TitleScreen;

//Load Music
//Music GameMusic;


//create image for icon

//Front ( included in Raylib-Master-Package and free to use even in commercial prjects)
Font customFont;

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	//init Screen && Sound
	initScreen();
	//init player
	initPlayer();

	//Load object textures

	//init Cells array with objects and logic
	initCells();
	// Update
	//update Point of View player
	player1.Player::SetDirectionState();

	//Monster
	Monster = LoadTexture("resources/Monster_Tiles.png");
	MonsterDespawn = LoadTexture("resources/Monster_Tiles_despawn.png");

	//Load fog textures
	cloud[0] = LoadTexture("resources/Cloud.png");
	cloud[1] = LoadTexture("resources/Cloud_Open.png");
	cloud[2] = LoadTexture("resources/Cloud_Open2.png");

	//load Rock
	Rock = LoadTexture("resources/Rock.png");

	//load character
	Hero = LoadTexture("resources/Character_Tileset.png");

	//load PowerUp
	PowerUp[0] = LoadTexture("resources/Heart.png");
	PowerUp[1] = LoadTexture("resources/Heart_empty.png");

	//load Medipack
	MediPack = LoadTexture("resources/Medipack.png");

	//LevelBackground
	LevelBackGround = LoadTexture("resources/BackGround2.png");

	//Health
	Health[0] = LoadTexture("resources/Health_3.png");
	Health[1] = LoadTexture("resources/Health_2.png");
	Health[2] = LoadTexture("resources/Health_1.png");

	//Light
	//Light = LoadTexture("resources/Light.png");

	//Sparkles
	Sparkles = LoadTexture("resources/Sparkles2.png");

	//Shield for UI
	ShieldIndicator = LoadTexture("resources/ShieldIndicator.png");

	//Shield item
	Shield = LoadTexture("resources/Shield_item.png");

	//lights
	Light_down = LoadTexture("resources/Light_down.png");
	Light_up = LoadTexture("resources/Light_up.png");
	Light_right = LoadTexture("resources/Light_right.png");
	Light_left = LoadTexture("resources/Light_left.png");

	//Ladder as Exit
	Ladder = LoadTexture("resources/Ladder.png");

	//Gold
	Gold = LoadTexture("resources/Gold.png");

	//Trap
	Trap = LoadTexture("resources/Trap.png");

	//TitleScreen
	TitleScreen = LoadTexture("resources/Title.png");

	//Icon
	//Icon = LoadTexture("resources/Health_3.png");

	//Load customFont
	customFont = LoadFont("resources/pixantiqua.ttf");

	//Load Sound
	//GameMusic = LoadMusicStream("resources/GameLoop2.ogg");

	//Retangle for level boundarys to check if player is inside or if there is a collision between them
	Rectangle Boundarys = { 0,0,getCurrentScreenWidthAsFloat(),getCurrentScreenHeightAsFloat() };





	//init layer0 with floor level textures
	//for (int i = 0; i < COLUMNS; i++)
	//	for (int j = 0; j < ROWS; j++)
	//{
	//	int randomNumber07 = GetRandomValue(0, 7);
	//
	//	
	//		//Fill Layer 0 with different floor textures
	//		switch (randomNumber07)
	//		{

	//		case 0:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 1:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 2:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 3:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 4:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 5:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 6:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		case 7:
	//			layer0Textures[i][j].currentTexture = LoadTexture("resources/FloorDirt.png");
	//			break;
	//		default:
	//			break;
	//		}
	//}


	//--------------------------------------------------------------------------------------
	// Main game loop
	while (!WindowShouldClose())
	{


		//Music
		/*PlayMusicStream(GameMusic);
		UpdateMusicStream(GameMusic);*/


		//CurrentScreen controls
		switch (currentScreen)
		{
		case TITLE:
			if (IsKeyPressed(KEY_W))
			{
				if (currentSelectedOption > 0)
					currentSelectedOption--;
			}
			if (IsKeyPressed(KEY_S))
			{
				if (currentSelectedOption < 3)
					currentSelectedOption++;
			}
			if (IsKeyPressed(KEY_ENTER))
			{
				switch (currentSelectedOption)
				{
				case 0:
					currentScreen = GAMEPLAY;
					break;
				case 1:
					currentScreen = MENU;
					break;
				case 2:
					currentScreen = CREDITS;
					break;
				case 3:
					CloseWindow();
					break;
				default:
					break;
				}

			}

			break;
		case MENU:

			if (IsKeyPressed(KEY_W))
			{
				if (currentSelectedSetting > 0)
					currentSelectedSetting--;
			}
			if (IsKeyPressed(KEY_S))
			{
				if (currentSelectedSetting < 3)
					currentSelectedSetting++;
			}
			if (IsKeyPressed(KEY_ENTER))
			{
				switch (currentSelectedSetting)
				{
				case 0:
					currentScreen = TITLE;
					break;
				case 1:
					//change language to german
					languageGerman = true;
					break;
				case 2:
					//gamepad active and installed
					/*if (IsGamepadAvailable)
					{
						controllerActive = true;
					}*/

					break;
				case 3:
					CloseWindow();
					break;
				default:
					break;
				}

			}
			break;

		case GAMEPLAY:
			////init Cells array with objects and logic
			//initCells();
			//// Update
			////update Point of View player
			//player1.Player::SetDirectionState();

		//CHEATS - DEVELOPER AND PRESENTATION FUNKTIONS
			if (IsKeyPressed(KEY_KP_2))
				player1.lifes--;
			if (IsKeyPressed(KEY_KP_1))
				player1.lifes++;
			if (IsKeyPressed(KEY_KP_3))
				player1.shield = true;
			if (IsKeyPressed(KEY_KP_4))
				player1.shield = false;
			if (IsKeyPressed(KEY_KP_5))
			{
				for (int i = 0; i < COLUMNS; i++)
				{
					for (int j = 0; j < ROWS; j++)
					{
						board[i][j].revealed = true;
					}
				}
			}
			if (IsKeyPressed(KEY_KP_6))
			{
				for (int i = 0; i < COLUMNS; i++)
				{
					for (int j = 0; j < ROWS; j++)
					{
						board[i][j].revealed = false;
					}
				}
			}


			//Update Frames / frameCounter
			frameTimer += GetFrameTime();
			if (frameTimer >= 0.2f)
			{
				frameTimer = 0.0f;
				currentFrame += 1;
			}
			currentFrame = currentFrame % maxFrames;


			//Collision Point of View and focused Cell and object actions
			for (int i = 0; i < COLUMNS; i++)
			{
				for (int j = 0; j < ROWS; j++)
				{//PickupGold
					if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 5)
					{
						player1.gold += 100;
						board[i][j].state = 6;
					}
					//if player is on trap lifes --
					if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 7)
					{
						if (player1.shield)//if player hast shield and Monster hit player, than remove shield instead of reduce life.
						{
							player1.shield = false;
						}
						else
						{
							player1.lifes--;
						}
						board[i][j].state = 8;
					}

					//PickUp medipack or if live full get score
					if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 3)
						if (player1.lifes >= 3)
						{
							player1.score += 100;
							board[i][j].state = 10;
						}
						else
						{
							player1.lifes++;
							board[i][j].state = 10;
						}
					//player finds shield. if player has shield + 200 pts
					if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 4)
						if (player1.shield)
						{
							player1.score += 200;
							board[i][j].state = 10;
						}
						else
						{
							player1.shield = true;
							board[i][j].state = 10;
						}

					if (CheckCollisionRecs(player1.vDirection, board[i][j].CellHitBox))//Player looks at Cell 
					{
						if (!board[i][j].revealed)
						{
							player1.collisionVDirection = true;
							if (IsKeyPressed(KEY_SPACE))
							{
								board[i][j].revealed = true; //Player points to unrevealed Cell and hits Space - If Cell !revealed => revealed.	
								player1.collisionVDirection = false;
								if (board[i][j].state == 1)
								{
									if (player1.shield)//if player hast shield and Monster hit player, than remove shield instead of reduce life.
									{
										player1.shield = false;
									}
									else
									{
										/*if ((GetRandomValue(0, 1)) == 0)
										{
											player1.lifes--;
										}
										else
										{
											player1.blocked = true;
										}*/
									}
									board[i][j].state = 2;
								}

							}
						}
						if (board[i][j].revealed)
							if (board[i][j].destructible)
							{
								player1.collisionVDirection = false;
							}
							else
							{
								player1.collisionVDirection = true;
							}
					}
				}
			}

			//Input Keyboard

			if (IsKeyPressed(KEY_D))
			{
				if (player1.posx < ((GetScreenWidth()) - (2 * (GetScreenWidth() / 32)))) //Player inside Boundarys-- if not dont move character
				{
					
						if (player1.directionState == 2 && !player1.collisionVDirection)//player looks in desired directon --move in that direction
						{
							player1.posx += player1.speed;
							player1.PlayerHitBox.x = player1.posx;
							player1.vDirection = { ((player1.posx + (3 * ((GetScreenWidth() / 32) / 2)))),(player1.posy + ((GetScreenHeight() / 18) / 2)) };
						}
						else// player looks in another direction - set new point of view
						{
							player1.directionState = 2;
							player1.PlayerHitBox.x = player1.posx;
							player1.vDirection = { ((player1.posx + (3 * ((GetScreenWidth() / 32) / 2)))),(player1.posy + ((GetScreenHeight() / 18) / 2)) };
						}
					
				}
			}
			if (IsKeyPressed(KEY_A))
			{
				if (player1.posx > (GetScreenWidth() / 32))
				{
					if (player1.directionState == 3 && !player1.collisionVDirection)
					{
						player1.posx -= player1.speed;
						player1.PlayerHitBox.x = player1.posx;
						player1.vDirection = { (player1.posx - ((GetScreenWidth() / 32) / 2)),(player1.posy + ((GetScreenHeight() / 18) / 2)) };
					}
					else
					{
						player1.directionState = 3;
						player1.PlayerHitBox.x = player1.posx;
						player1.vDirection = { (player1.posx - ((GetScreenWidth() / 32) / 2)),(player1.posy + ((GetScreenHeight() / 18) / 2)) };
					}
				}
			}
			if (IsKeyPressed(KEY_W))
			{
				if (player1.posy > (GetScreenHeight() / 18))
				{
					if (player1.directionState == 1 && !player1.collisionVDirection)
					{
						player1.posy -= player1.speed;
						player1.PlayerHitBox.y = player1.posy;
						player1.vDirection = { ((player1.posx + ((GetScreenWidth() / 32) / 2))),(player1.posy - ((GetScreenHeight() / 18) / 2)) };
					}
					else
					{
						player1.directionState = 1;
						player1.PlayerHitBox.y = player1.posy;
						player1.vDirection = { ((player1.posx + ((GetScreenWidth() / 32) / 2))),(player1.posy - ((GetScreenHeight() / 18) / 2)) };
					}

				}
			}
			if (IsKeyPressed(KEY_S))
			{
				if (player1.posy < (GetScreenHeight()) - (2 * (GetScreenHeight() / 18)))
				{
					if (player1.directionState == 0 && !player1.collisionVDirection)
					{
						player1.posy += player1.speed;
						player1.PlayerHitBox.y = player1.posy;
						player1.vDirection = { (player1.posx + ((GetScreenWidth() / 32) / 2)),(player1.posy + (3 * (GetScreenHeight() / 18) / 2)) };
					}
					else
					{
						player1.directionState = 0;
						player1.PlayerHitBox.y = player1.posy;
						player1.vDirection = { (player1.posx + ((GetScreenWidth() / 32) / 2)),(player1.posy + (3 * (GetScreenHeight() / 18) / 2)) };
					}

				}
			}
			break;
		case CREDITS:
			break;
		case GAMEOVER:
			break;
		default:
			//currentScreen = TITLE;
			break;
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(DARKGRAY);

		switch (currentScreen)
		{
		case TITLE:
			//Background
			DrawTexture(TitleScreen, 0, 0, WHITE);
			//Title
			DrawRectangle((GetScreenWidth() / 2), 60, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 70, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ 1020.0,120.0 }, 120, 0, BLACK);

			//MenuButtons
			//StartGame
			DrawRectangle(1500, 650, 390, 60, BLACK);
			DrawRectangle(1510, 660, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("START GAME")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
			//Controls/Settings
			DrawRectangle(1500, 730, 390, 60, BLACK);
			DrawRectangle(1510, 740, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SETTINGS")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
			//Credits/Highscore
			DrawRectangle(1500, 810, 390, 60, BLACK);
			DrawRectangle(1510, 820, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("CREDITS/Stats")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
			//Quit
			DrawRectangle(1500, 890, 390, 60, BLACK);
			DrawRectangle(1510, 900, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
			//Highlighting Selection
			switch (currentSelectedOption)
			{
			case 0:
				DrawRectangle(1510, 660, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("START GAME")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
				break;
			case 1:
				DrawRectangle(1510, 740, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("SETTINGS")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
				break;
			case 2:
				DrawRectangle(1510, 820, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("CREDITS/Stats")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
				break;
			case 3:
				DrawRectangle(1510, 900, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
				break;
			default:
				break;
			}

			break;
		case MENU:
			//Background
			DrawTexture(TitleScreen, 0, 0, WHITE);
			//MenuButtons
			//Back to title
			DrawRectangle(1500, 650, 390, 60, BLACK);
			DrawRectangle(1510, 660, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("BACK")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
			//change language to german 
			DrawRectangle(1500, 730, 390, 60, BLACK);
			DrawRectangle(1510, 740, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("LANGUAGE")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
			//init gamepad
			DrawRectangle(1500, 810, 390, 60, BLACK);
			DrawRectangle(1510, 820, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("INIT CONTROLLER")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
			//Quit Game
			DrawRectangle(1500, 890, 390, 60, BLACK);
			DrawRectangle(1510, 900, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
			//Highlighting Selection
			switch (currentSelectedSetting)
			{
			case 0:
				DrawRectangle(1510, 660, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("BACK")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
				break;
			case 1:
				DrawRectangle(1510, 740, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("LANGUAGE")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
				break;
			case 2:
				DrawRectangle(1510, 820, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("INIT CONTROLLER")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
				break;
			case 3:
				DrawRectangle(1510, 900, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
				break;
			default:
				break;
			}
			break;
		case GAMEPLAY:

			// Level Background
			DrawTexture(LevelBackGround, 0, 0, RAYWHITE);

			//Draw Cells
			for (int i = 0; i < COLUMNS; i++)
			{
				for (int j = 0; j < ROWS; j++)
				{
					DrawTexture(layer0Textures[i][j].currentTexture, board[i][j].posx, board[i][j].posy, RAYWHITE);
					if (board[i][j].revealed) //draw this if cell is revealed
					{
						switch (board[i][j].state)
						{
						case 0://solid - Rock
							DrawTexture(Rock, board[i][j].posx, board[i][j].posy, RAYWHITE);
							break;
						case 1:// negative - Monster
							DrawTextureRec(Monster, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ board[i][j].posx,board[i][j].posy }, WHITE);
							break;
						case 2:// negative - Monster despawn 		
							if (board[i][j].animationCounter < 32)
							{
								DrawTextureRec(Monster, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ board[i][j].posx,board[i][j].posy }, WHITE);
								board[i][j].animationCounter++;
							}
							else 
							{
								if (board[i][j].animationCounter >= 32 && board[i][j].animationCounter != 64)
								{
									DrawTextureRec(MonsterDespawn, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ board[i][j].posx,board[i][j].posy }, WHITE);
									board[i][j].animationCounter++;
								}

							}
							break;
						case 3://Medipack
							DrawTexture(MediPack, board[i][j].posx, board[i][j].posy, RAYWHITE);
							break;
						case 4://Shield
							DrawTexture(Shield, board[i][j].posx, board[i][j].posy, RAYWHITE);
							break;
						case 5://Gold
							DrawTextureRec(Gold, Rectangle{ 0,0,(float)(Gold.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
							break;
						case 6://Gold picked up (gold despawn = true setzen wenn player hit, dann wie bei monster)
							if (board[i][j].animationCounter < 32)
							{
								DrawTextureRec(Gold, Rectangle{ 0.0f,0,(float)(Gold.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
								board[i][j].animationCounter++;
							}
							else
							{
								if (board[i][j].animationCounter >= 32 && board[i][j].animationCounter != 64)
								{
									DrawTextureRec(Gold, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Gold.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
									board[i][j].animationCounter++;
								}
							}
							break;
						case 7://negative - hiddenTrap hidden
							DrawTextureRec(Ladder, Rectangle{ 0,0,(float)(Trap.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
							break;
						case 8://negative - hiddenTrap active
							DrawTextureRec(Trap, Rectangle{ (float)(currentFrame * 60),0,(float)(Trap.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
							break;
						case 9://Exit 
							DrawTexture(Ladder, board[i][j].posx, board[i][j].posy, RAYWHITE);
							break;

						default:// >=10 -> Empty field. Only floor layer visible plus Fog of War
							break;
						}
						//DrawTexture(cloud[0], board[i][j].posx, board[i][j].posy, RAYWHITE);

					}
					else //draw fog if Cell is not revealed
					{
						if (!board[i][j].revealed)
							DrawTexture(cloud[2], board[i][j].posx, board[i][j].posy, RAYWHITE);
						//DrawRectangle(board[i][j].posx+10, board[i][j].posy+10, (board[i][j].width-20), (board[i][j].height-20), BLACK);
					}

					//Sparkles as Hint for objects behind fog
					if (!board[i][j].revealed && board[i][j].state < 10 && board[i][j].state != 2 && board[i][j].state != 6 && board[i][j].state != 8)
					{
						DrawTextureRec(Sparkles, Rectangle{ (float)(currentFrame * 60),0,(float)(Sparkles.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
						//DrawCircle(board[i][j].posx+30, board[i][j].posy+30, 11, GOLD);
					}
					//Block /Miss Mechanic
					/*if (player1.blocked)
					{
						player1.blocked = false;
					}*/

					//if (board[i][j].animationCounter >= 128 && board[i][j].animationCounter != 256)
					//{
					//	//DrawTextEx(customFont, (FormatText("START GAME")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
					//	DrawText("MISS", player1.posx, player1.posy, 10, RED);
					//	//DrawTextureRec(Gold, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Gold.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
					//	board[i][j].animationCounter++;
					//}

				}

			}
				//Draw Level Boundarys and PlayerHitBox(around Player model and own Cell)
				for (int i = 0; i <= 32; i++)
				{
					//Boundary
					DrawRectangleLinesEx(Boundarys, (GetScreenHeight() / 18), BLANK);
					//PlayerHitBox
					DrawRectangleLinesEx(player1.PlayerHitBox, 10, BLANK);
				}

				//Dirty Little Helper: Set to color to see point of view of character
				//As Point of View HitBox(defines Point of View direction for movment logic)
				//Used to Check if player looks at solid or weak structure. Solid objects shouldn't be passable. (Fog is solid until revealed)
				//DrawCircle(player1.vDirection.x,player1.vDirection.y,5,WHITE);

				//Draw Hero and Light

				switch (player1.directionState)
				{
				case 0://Down
					DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
					if (IsKeyDown(KEY_SPACE))
					{
						DrawTexture(Light_down, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
					}
					break;
				case 1://Up
					DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),60,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
					if (IsKeyDown(KEY_SPACE))
					{
						DrawTexture(Light_up, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
					}
					break;
				case 2://Right
					DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),120,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
					if (IsKeyDown(KEY_SPACE))
					{
						DrawTexture(Light_right, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
					}
					break;
				case 3://Left
					DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),180,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
					if (IsKeyDown(KEY_SPACE))
					{
						DrawTexture(Light_left, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
					}
					break;
				default:
					DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
					break;
				}


				//UI -Elements

				//Score
				DrawTextEx(customFont, (FormatText("Score: %06i", player1.score)), Vector2{ (16 * (float)(GetScreenWidth() / 32) + 10) ,(17 * (float)(GetScreenHeight() / 18) + 10) }, 20, 0, RED);

				//Gold
				DrawTextEx(customFont, (FormatText("Gold: %08i", player1.gold)), Vector2{ (8 * (float)(GetScreenWidth() / 32) + 10), (17 * (float)(GetScreenHeight() / 18) + 10) }, 20, 0, GOLD);



				//Health - Hearts and Shield
				//Hard - start 3 lifes no shield
				//medium 4 hearts no shield
				//easy 5 hearts shield
				switch (player1.lifes)
				{
				case 1:

					if (player1.shield)
					{
						DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					}
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);

					break;
				case 2:
					if (player1.shield)
					{
						DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					}
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					break;
				case 3:
					if (player1.shield)
					{
						DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					}
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);
					break;
				default:
					break;
				}

				//Health
				if (player1.lifes == 3)
					DrawTexture(Health[0], (15 * (GetScreenWidth() / 32)), (17 * (GetScreenHeight() / 18)), RAYWHITE);
				if (player1.lifes == 2)
					DrawTexture(Health[1], (15 * (GetScreenWidth() / 32)), (17 * (GetScreenHeight() / 18)), RAYWHITE);
				if (player1.lifes == 1)
					DrawTexture(Health[2], (15 * (GetScreenWidth() / 32)), (17 * (GetScreenHeight() / 18)), RAYWHITE);
				//if (player1.lifes==0)
				//GameState = GameOver;



			break;
		case CREDITS:
			break;
		case GAMEOVER:
			break;
		default:
			//currentScreen = TITLE;
			break;
			
		}
	}

EndDrawing();
//----------------------------------------------------------------------------------

// De-Initialization
//--------------------------------------------------------------------------------------   
//Unload Funktions schreiben
//Unload Textures
//Unload Music
//Unload Fx
CloseWindow();
//--------------------------------------------------------------------------------------
return 0;
}

void initScreen()
{
	std::cout << "intiScreenAnfang" << std::endl;
	int screenWidth = 1920;
	int screenHeight = 1080;
	//ToggleFullscreen();
	InitWindow(screenWidth, screenHeight, "Sweet Dreams");
	SetTargetFPS(60);
	//InitAudioDevice();
	//SetWindowIcon();
	//SetWindowTitle();
	std::cout << "intiScreenEnde"<<std::endl;
}



void initCells()
{
	//Cell Array 
	int exitCounter = 0;
	int trapCounter = 0;
	std::cout << "intitCellsAnfang" << std::endl;
	for (int i = 0; i < COLUMNS; i++)
	{
		std::cout << "intitCellsAnfang i: " <<i<< std::endl;

		for (int j = 0; j < ROWS; j++)
		{
			std::cout << "intitCellsAnfang j: " << j << std::endl;

			board[i][j].Cell::Cell();

			//Cell Positioning Data for Drawing (Fills feld with objects or leaves them free)
			board[i][j].posx = ((board[i][j].firstCellPosX) + (i * board[i][j].width));
			board[i][j].posy = (board[i][j].firstCellPosY) + (j * board[i][j].height);
			board[i][j].CellHitBox.x = board[i][j].posx;
			board[i][j].CellHitBox.y = board[i][j].posy;


			//Hard //Medium // Easy
			int rnd = GetRandomValue(0, (10 * currentDifficulty));
			if (rnd == 6 || rnd == 2 || rnd == 8)// if despawn state of object, reset to spawn state
				rnd--;
			board[i][j].state = rnd;

			if (board[i][j].state == 0) // Solid Rock
			{
				board[i][j].destructible = false;//player cannot move through Rocks
			}
			// state 2 == monster despawn animation state
			if (board[i][j].state == 1) // Monster
			{
				board[i][j].hasBomb = true; //does damage to player if fog on field is revealed through player interaction
			}
			//state 3 == Medipack. funktion handled in player collision. has no extra statusvariable
			if (board[i][j].state == 4) //Shields player and protects from next source of damage
			{
				board[i][j].isShield = true;
			}
			if (board[i][j].state == 5)
			{
				board[i][j].isGold = true; //adds 100 gold pieces to player money/inventory
			}
			if (board[i][j].state == 7)//sets field to hidden trap 
			{
				if (trapCounter < 3)
				{
					board[i][j].isTrap = true;
					trapCounter++;
				}
				else
				{
					board[i][j].state = 10;
				}
			}
			if (board[i][j].state == 9)
			{
				if (exitCounter == 0)
				{
					board[i][j].isExit = true; //ends current level and changes gamestate to level statistics screen
					exitCounter++;
				}
				else
				{
					board[i][j].state++;
				}
			}
			



			//neu wahrscheinilch sinnlos
			//if (!board[i][j].revealed)
			//{
			//	//board[i][j].destructible == false;
			//}
			//neu
			if ((player1.PlayerHitBox.x == board[i][j].posx) && (player1.PlayerHitBox.y == board[i][j].posy))

			{
				//prüfen ob feld unter startfeld vorhanden, wenn ja mache feld leer und decke auf 
				board[i][j].state = 10;
				board[i][j].revealed = true;
				board[i][j].destructible = true;
				std::cout << "board[" << i << "][" << j << "] set to NULL because of Player pressence!!" << std::endl;
			}
		}
	}
	std::cout << "intitCellsAnfangEnde" << std::endl;
}


float getCurrentScreenWidthAsFloat()
{
	return GetScreenWidth();
}
float getCurrentScreenHeightAsFloat()
{
	return GetScreenHeight();
}

void initPlayer()
{
	player1.Player::Player();

}