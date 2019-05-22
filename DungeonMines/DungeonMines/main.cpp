#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
//Header files
#include "Cell.h"
#include "player.h"
//#include "textures.h"
//represents the level
#define COLUMNS 30
#define ROWS 16 
//animation settings
#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
//max input chars
#define MAXINPUT 20


//ScreenSize
int screenWidth = 1920;
int screenHeight = 1080;

//GameOver
bool isGameOver = false;

//Win
bool isVictory = false;

//Pause Funktion
bool Pause = false;

//ScreenSelection
typedef enum GameScreen { TITLE, MENU, GAMEPLAY, CREDITS, GAMEOVER, STATISTICS, LOGO}GameScreen;
GameScreen currentScreen = LOGO;
int currentSelectedOption = 0;
int currentSelectedSetting = 0;
//languagecontrol
bool languageGerman = false;

//forward declarations for funktions
void initScreen();
void initPlayer();
void initCells();
float getCurrentScreenWidthAsFloat();
float getCurrentScreenHeightAsFloat();



//Highscore
int highscoreOld = 1;
int highscoreNew = 0;
char highscoreNameOld[20] = "HaraldAsChar\0";
char highscoreNameNew[20] = "\0";
int inputCounter = 0;

//frames
float framewidth = (float)(GetScreenWidth() / 32);

//amount of pictures per tileset
int maxFrames = 4;

//timer for animation speed
float frameTimer = 0.0f;
int currentFrame = 0;

//holds active Level
int currentLevel = 1;

//displaytime for Intro
int introTimer = 0;

//holds active difficulty modifier
//Difficulty Settings as modificator for increasing difficulty durig progress
//no Easy, because easy settings are the defaults
//5 == easy ; medium = easy*2, hard = easy *3 
int currentDifficulty = 5;

//ingame object quantity
int maxFieldsPerLevel = COLUMNS * ROWS;

//For Statistics
int fieldsRevealed = 0;
int lifesLost = 0;
int medipacksCollected = 0;
int shieldsCollected = 0;
int goldCollected = 0;
int levelScore = 0;
int monsterFound = 0;
int trapsActivated = 0;

//Create objects
Player player1;
Cell board[30][16];

//Image
Image icon;

//create Textureobjects
Texture2D Monster;
Texture2D cloud[3];
Texture2D Rock;
Texture2D Hero;
Texture2D PowerUp[2];
Texture2D LevelBackGround;
Texture2D Health[3];
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
Texture2D TitleSleep;
Texture2D GrayBackground;
Texture2D GameOverScreen;

//Create Music Objects
Music TitleMusic;
Music CreditsMusic;
Music GameLoop;
Music GameLoop2;
Music GameLoop3;

//Create Sound Objects
Sound fxActivate;
Sound fxMoveSelection;
Sound fxFlashlight;
Sound fxGetItem;
Sound fxGetGold;
Sound fxExplode;
Sound fxGetDamage;
Sound fxActivateTrap;
Sound fxLevelExit;
Sound fxPlayerBlocked;
Sound fxGameOverScream;
Sound fxRevealDarkness;
Sound fxFootSteps;


//Front ( included in Raylib-Master-Package and free to use even in commercial projects)
Font customFont;

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------

	//init Screen && Sound
	initScreen();
	
	//init player
	initPlayer();

	//init Cells array with objects and logic
	initCells();

	// Update
	//update Point of View player
	player1.Player::SetDirectionState();
	
	//Icon
	icon = LoadImage("resources/Health_2.png");

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

	//TitleSleepAnimation
	TitleSleep = LoadTexture("resources/TitleSleep.png");

	//GrayBack
	GrayBackground = LoadTexture("resources/GreyBackground.png");

	//GameOverScreen
	GameOverScreen = LoadTexture("resources/GameOverScreen.png");

	//Load customFont
	customFont = LoadFont("resources/pixantiqua.ttf");

	//Load Music Files
	TitleMusic = LoadMusicStream("resources/Music/GameLoop2.ogg");
	CreditsMusic = LoadMusicStream("resources/Music/Credits.ogg");
	GameLoop = LoadMusicStream("resources/Music/GameLoopRock.ogg");
	GameLoop2 = LoadMusicStream("resources/Music/GameLoopRockDistortionHalf.ogg");
	GameLoop3 = LoadMusicStream("resources/Music/GameLoopRockDistortionBlast.ogg");

	//Load Sound Files - Sounds = Example Sounds from Raylib
	fxActivate = LoadSound("resources/Sounds/start.wav");
	fxMoveSelection = LoadSound("resources/Sounds/sample_on.wav");
	fxFlashlight = LoadSound("resources/Sounds/fx_button.wav");
	fxRevealDarkness = LoadSound("resources/Sounds/Wind2.wav");
	fxGetItem = LoadSound("resources/Sounds/win.wav");
	fxGetGold = LoadSound("resources/Sounds/coin.wav");
	fxExplode = LoadSound("resources/Sounds/boom.wav");
	fxGetDamage = LoadSound("resources/Sounds/pause.wav");
	fxActivateTrap = LoadSound("resources/Sounds/spring.wav");
	fxLevelExit = LoadSound("resources/Sounds/buttonfx.wav");
	fxPlayerBlocked = LoadSound("resources/Sounds/dash.wav");
	fxGameOverScream = LoadSound("resources/Sounds/die.wav");
	fxFootSteps = LoadSound("resources/Sounds/fx_leave.wav");

	//Icon
	SetWindowIcon(icon);

	//Retangle for level boundarys to check if player is inside or if there is a collision between them
	Rectangle Boundarys = { 0,0,getCurrentScreenWidthAsFloat(),getCurrentScreenHeightAsFloat() };

	//Messages Storyteller/Narrator
	int textSpeed = 0;
	const char titleMessage[273] = "Every Hero needs beauty sleep. Here is one of them. Not the best ohne, that's for\nshure. But at least one of them. This is Beardman. He has a beard and\n...yeah that's it I think.....\nEnough said I think everybody of you wants to know what kind of dreams a hero has..";
	const char titleMessageGerman[298] = "Jeder Held brauchst seinen Schönheitsschlaf. Hier ist einer von ihnen.\nSicherliche nicht der Beste, das ist sicher. Aber besser als keiner.\nDas ist Bartmann. Er hat einen Bart und...nun ja das war es auch schon.....\nGenug gesagt ich denke jeder von euch möchte wissen was so ein Held so traeumt...";
	const char statisticsMessage[] = "You reached for the light above and relized that this wasn't the exit.";
	const char statisticsMessageGerman[] = "Du erreichst die Quelle des Lichts und musst mit Erschrecken feststellen,\ndass das nicht das Ende war.";
	const char gameOverMessage[88] = "What happened?\nWhere am I?\nWhat a dream.....\nIt was just a Dream......\nOr not?......";
	const char gameOverMessageGerman[129] = "Was ist passiert??\nWo bin ich überhaupt?\nWas fuer ein Traum......\nZum Glueck war das nur ein Traum....\nOder doch nicht?......";


	//--------------------------------------------------------------------------------------
	// Main game loop
	while (!WindowShouldClose())
	{
		//Hide Cursor, because mouse is not needed for controlling the game
		HideCursor();
		DisableCursor();

		//init Highscore
		std::ifstream readScore;
		readScore.open("resources/Highscore.txt");
		readScore >> highscoreNameOld >> highscoreOld;
		readScore.close();

		//CurrentScreen controls and updates
		switch (currentScreen)
		{
		case LOGO:
		{
			//Show intro for x seconds to give game time to load

			introTimer++;
			if (((introTimer / 120) % 2) == 1)
			{
				currentScreen = TITLE;
				introTimer = 0;
			}
		}
		break;
		case TITLE:
		{
			//start playing music
			StopMusicStream(CreditsMusic);
			PlayMusicStream(TitleMusic);
			UpdateMusicStream(TitleMusic);

			if (IsKeyPressed(KEY_W))
			{
				PlaySound(fxMoveSelection);
				if (currentSelectedOption > 0)
					currentSelectedOption--;
			}
			if (IsKeyPressed(KEY_S))
			{
				PlaySound(fxMoveSelection);
				if (currentSelectedOption < 3)
					currentSelectedOption++;
			}
			if (IsKeyPressed(KEY_SPACE))
			{
				PlaySound(fxActivate);

				switch (currentSelectedOption)
				{
				case 0:
					if (isGameOver)
					{
						//init New Level
						initPlayer();
						//init Cells array with objects and logic
						initCells();
						isGameOver = false;
						Pause = false;
						inputCounter = 0;				
					}							
					currentScreen = GAMEPLAY;
					break;
				case 1:
					currentScreen = MENU;
					break;
				case 2:
					currentScreen = CREDITS;
					break;
				case 3:
					CloseAudioDevice();
					CloseWindow();
					exit(EXIT_SUCCESS);
					break;
				default:
					break;
				}

			}
			//frametimer for sleep animation
			frameTimer += GetFrameTime();
			if (frameTimer >= 0.6f)
			{
				frameTimer = 0.0f;
				currentFrame += 1;
			}
			currentFrame = currentFrame % maxFrames;
		}
		break;
		case MENU:
		{
			StopMusicStream(CreditsMusic);
			UpdateMusicStream(TitleMusic);

			if (IsKeyPressed(KEY_W))
			{
				PlaySound(fxMoveSelection);
				if (currentSelectedSetting > 0)
					currentSelectedSetting--;
			}
			if (IsKeyPressed(KEY_S))
			{
				PlaySound(fxMoveSelection);
				if (currentSelectedSetting < 1)
					currentSelectedSetting++;
			}
			if (IsKeyPressed(KEY_SPACE))
			{
				PlaySound(fxActivate);
				switch (currentSelectedSetting)
				{
				case 0:
					currentScreen = TITLE;
					break;
				case 1:
					//change language to german
					if (languageGerman)
					{
						languageGerman = false;
					}
					else
					{
						languageGerman = true;
					}				
					break;
				default:
					break;
				}

			}
		
			//frametimer for sleep animation
			frameTimer += GetFrameTime();
			if (frameTimer >= 0.6f)
			{
				frameTimer = 0.0f;
				currentFrame += 1;
			}
			currentFrame = currentFrame % maxFrames;
		}
		break;
		case GAMEPLAY:
		{

			// Update
			//Music selection per level
			StopMusicStream(TitleMusic);
			if (currentLevel == 1)
			{
				PlayMusicStream(GameLoop);
				UpdateMusicStream(GameLoop);
			}
			if (currentLevel == 2)
			{
				PlayMusicStream(GameLoop2);
				UpdateMusicStream(GameLoop2);
			}
			if (currentLevel >= 3)
			{
				PlayMusicStream(GameLoop3);
				UpdateMusicStream(GameLoop3);
			}
			
		
				//update Point of View player
				player1.Player::SetDirectionState();

				//Pause, Game Over Control and Victory Screen Controls
				if (IsKeyPressed(KEY_P))
				{
					if (Pause == false)
					{
						Pause = true;
					}
					else
					{
						Pause = false;
					}
				}
	
				if (IsKeyPressed(KEY_ENTER))
				{
					if (Pause)
					{
						if (isGameOver)
						{
							currentScreen = GAMEOVER;
						}
						if (isVictory)
						{
							//NextLevel
							currentScreen = STATISTICS;
						}
						if (!isGameOver && !isVictory)
						{
							currentScreen = TITLE;
						}
					}

				}
				
				//CHEATS - DEVELOPER AND PRESENTATION FUNKTIONS
				if (IsKeyPressed(KEY_KP_1))
					player1.lifes--;
				if (IsKeyPressed(KEY_KP_2))
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
				if (IsKeyPressed(KEY_KP_7))
				{
					player1.score += 100;
				}


				//Update Frames / frameCounter

				if (!Pause)
				{
					frameTimer += GetFrameTime();
					if (frameTimer >= 0.2f)
					{
						frameTimer = 0.0f;
						currentFrame += 1;
					}
					currentFrame = currentFrame % maxFrames;
				}


				//Collision Point of View and focused Cell and object actions
				for (int i = 0; i < COLUMNS; i++)
				{
					for (int j = 0; j < ROWS; j++)
					{
						//Victory
						if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 9)
						{																												
							isVictory = true;
							Pause = true;
						}
																		
						//PickupGold
						if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 5)
						{
							goldCollected++;
							PlaySound(fxGetGold);
							player1.gold += 100;
							board[i][j].state = 6;
						}
						//if player is on trap lifes -- / shield destroyed
						if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 7)
						{
							trapsActivated++;
							if (player1.shield)//if player hast shield and Monster hit player, than remove shield instead of reduce life.
							{
								PlaySound(fxPlayerBlocked);
								player1.shield = false;
							}
							else
							{
								PlaySound(fxGetDamage);
								player1.lifes--;
							}
							board[i][j].state = 8;
						}

						//PickUp medipack or if live full get score
						if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 3)
						{
							medipacksCollected++;
							if (player1.lifes >= 3)
							{
								PlaySound(fxGetItem);
								player1.score += 100;
								board[i][j].state = 10;
							}
							else
							{
								PlaySound(fxGetItem);
								player1.lifes++;
								board[i][j].state = 10;
							}
						}
						//player finds shield. if player has shield + 200 pts
						if (player1.posx == board[i][j].posx && player1.posy == board[i][j].posy && board[i][j].state == 4)
						{
							shieldsCollected++;
							if (player1.shield)
							{
								PlaySound(fxGetItem);
								player1.score += 200;
								board[i][j].state = 10;
							}
							else
							{
								PlaySound(fxGetItem);
								player1.shield = true;
								board[i][j].state = 10;
							}
						}

						if (CheckCollisionRecs(player1.vDirection, board[i][j].CellHitBox))//Player looks at Cell 
						{
							if (!board[i][j].revealed)
							{
								player1.collisionVDirection = true;
								if (IsKeyPressed(KEY_SPACE))
								{
									fieldsRevealed++;
									PlaySound(fxRevealDarkness);
									board[i][j].revealed = true; //Player points to unrevealed Cell and hits Space - If Cell !revealed => revealed.	
									player1.collisionVDirection = false;
									if (board[i][j].state == 1)
									{
										monsterFound++;
										PlaySound(fxExplode);
										if (player1.shield)//if player has shield and Monster hit player, than remove shield instead of reduce life.
										{
											PlaySound(fxPlayerBlocked);
											player1.shield = false;
										}
										else
										{
											if ((GetRandomValue(0, 1)) == 0)
											{
												PlaySound(fxGetDamage);
												player1.lifes--;
											}
											else
											{
												PlaySound(fxPlayerBlocked);
												player1.blocked = true;
											}
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
				if (!Pause)
				{
					if (IsKeyPressed(KEY_D))
					{
						PlaySound(fxFootSteps);
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
						PlaySound(fxFootSteps);

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
						PlaySound(fxFootSteps);

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
						PlaySound(fxFootSteps);

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
					if (player1.lifes == 0)
					{
						PlaySound(fxGameOverScream);
						isGameOver = true;
						Pause = true;
					}
				}
			}		
		break;
		case STATISTICS:
		{
			StopMusicStream(GameLoop);
			PlayMusicStream(TitleMusic);
			UpdateMusicStream(TitleMusic);
			if (isVictory)
			{
				//init new level with increased difficulty
				currentLevel++;
				if (currentDifficulty != 1)
				{
					currentDifficulty -= 2;
				}
				player1.SetStartingPosx();
				player1.SetStartingPosy();
				initCells();
				Pause = false;
				isVictory = false;
			}
			if (IsKeyPressed(KEY_SPACE))
			{				
				currentScreen = GAMEPLAY;
			}
			if (IsKeyPressed(KEY_ENTER))
			{
				if (player1.shield)
				{

				}
				else
				{
					player1.shield = true;
					player1.gold -= 500;
				}
			}
		}
		break;
		case CREDITS:
		{
			StopMusicStream(TitleMusic);
			PlayMusicStream(CreditsMusic);
			UpdateMusicStream(CreditsMusic);

			if (IsKeyPressed(KEY_SPACE))
			{
				PlaySound(fxActivate);
				currentScreen = TITLE;
			}
		}
		break;
		case GAMEOVER:
		{
			StopMusicStream(GameLoop);
			PlayMusicStream(TitleMusic);
			UpdateMusicStream(TitleMusic);
			if (IsKeyPressed(KEY_SPACE))
			{
				//Reset Playerstats for init new game
				PlaySound(fxActivate);
				currentScreen = TITLE;				
				initPlayer();
				currentDifficulty = 5;
				Pause = false;
				currentLevel = 1;				
			}

			//Logic for input new highscore //Raylib examples
			if (player1.score > highscoreOld)
			{
				
					int key = GetKeyPressed();

					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (inputCounter < MAXINPUT))
					{
						highscoreNameNew[inputCounter] = (char)key;
						inputCounter++;
					}
				
					if (IsKeyPressed(KEY_ENTER))
					{
						highscoreNew = player1.score;
						//write new highscore to textfile
						std::ofstream writeScore;	
						writeScore.open("resources/Highscore.txt");
						writeScore << highscoreNameNew <<std::endl<< highscoreNew;						
						writeScore.close();						
					}
			}
		}
		break;
		default:
			break;
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		
		ClearBackground(RAYWHITE);

		switch (currentScreen)
		{
		case LOGO:
		{
			DrawRectangle(screenWidth / 2 - 128, screenHeight / 2 - 128, 256, 256, BLACK);
			DrawRectangle(screenWidth / 2 - 112, screenHeight / 2 - 112, 224, 224, RAYWHITE);
			DrawText("raylib", screenWidth / 2 - 44, screenHeight / 2 + 48, 50, BLACK);
		}
		break;
		case TITLE:
		{
			//Background
			//for frametimer sleep animation
			if (currentFrame == 0)
			{
				DrawTexture(TitleSleep, 0, 0, WHITE);
			}
			else
			{
				DrawTexture(TitleScreen, 0, 0, WHITE);
			}

			//GameTitle Textfield
			DrawRectangle((GetScreenWidth() / 2), 60, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 70, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ 1020.0,120.0 }, 120, 0, BLACK);

			//DialogeBox
			DrawRectangle((GetScreenWidth() / 2), 360, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 370, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			

			//Highscore Textfield
			DrawRectangle((GetScreenWidth()/2),((GetScreenHeight()/2)+110),500,300, BLACK);
			DrawRectangle(((GetScreenWidth() / 2)+20), ((GetScreenHeight() / 2)+130),460,260,DARKPURPLE);

			//Highscore Text
			DrawTextEx(customFont, (FormatText("Best Dreamer")), Vector2{ (float)((GetScreenWidth() / 2) + 50),(float)((GetScreenHeight() / 2) + 150) }, 30, 0, BLACK);
			DrawText(highscoreNameOld, ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 200),50,BLACK);
			DrawText(FormatText("%8i",highscoreOld), ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 300), 50, BLACK);


			//MenuButtons
			//StartGame Button
			DrawRectangle(1500, 650, 390, 60, BLACK);
			DrawRectangle(1510, 660, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("START GAME")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
			//Controls/Settings Button
			DrawRectangle(1500, 730, 390, 60, BLACK);
			DrawRectangle(1510, 740, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SETTINGS")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
			//Credits Button
			DrawRectangle(1500, 810, 390, 60, BLACK);
			DrawRectangle(1510, 820, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("CREDITS")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
			//Quit Button
			DrawRectangle(1500, 890, 390, 60, BLACK);
			DrawRectangle(1510, 900, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
			//Infomation
			DrawText("Use WASD to navigate", 1000, 970, 20, BLACK);
			DrawText("Press SPACE to confirm", 1000, 990, 20, BLACK);
			//Storyteller
			DrawText("Narrathor: ",995,375,20,WHITE);
			if (languageGerman)
			{
				DrawText(TextSubtext(titleMessageGerman, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}
			else
			{
				DrawText(TextSubtext(titleMessage, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}
			
			//Highlighting Selected Button
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
				DrawTextEx(customFont, (FormatText("CREDITS")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
				break;
			case 3:
				DrawRectangle(1510, 900, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
				break;
			default:
				break;
			}
		}
		break;
		case MENU:
		{

			//Background
			//frametimer sleep animation
			if (currentFrame == 0)
			{
				DrawTexture(TitleSleep, 0, 0, WHITE);
			}
			else
			{
				DrawTexture(TitleScreen, 0, 0, WHITE);
			}

			//GameTitle
			DrawRectangle((GetScreenWidth() / 2), 60, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 70, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ 1020.0,120.0 }, 120, 0, BLACK);

			//Infomation
			DrawText("Use WASD to navigate", 1000, 970, 20, BLACK);
			DrawText("Press SPACE to confirm", 1000, 990, 20, BLACK);

			
			//MenuButtons
			//Back to title
			DrawRectangle(1500, 650, 390, 60, BLACK);
			DrawRectangle(1510, 660, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("BACK")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
			//change language to german 
			DrawRectangle(1500, 730, 390, 60, BLACK);
			DrawRectangle(1510, 740, 370, 40, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("LANGUAGE")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);			
			switch (currentSelectedSetting)
			{
			case 0:
			{
				DrawRectangle(1510, 660, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("BACK")), Vector2{ 1590.0,665.0 }, 35, 0, BLACK);
			}
			break;
			case 1:
			{
				DrawRectangle(1510, 740, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("LANGUAGE")), Vector2{ 1590.0,745.0 }, 35, 0, BLACK);
			}
			break;
			case 2:
			{
				DrawRectangle(1510, 820, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("INIT CONTROLLER")), Vector2{ 1590.0,825.0 }, 35, 0, BLACK);
			}
			break;
			case 3:
			{
				DrawRectangle(1510, 900, 370, 40, WHITE);
				DrawTextEx(customFont, (FormatText("QUIT GAME")), Vector2{ 1590.0,905.0 }, 35, 0, BLACK);
			}
			break;
			default:
				break;
			}
		}
		break;
		case GAMEPLAY:
		{

			// Level Background
			DrawTexture(LevelBackGround, 0, 0, RAYWHITE);

			//Draw Cells
			for (int i = 0; i < COLUMNS; i++)
			{
				for (int j = 0; j < ROWS; j++)
				{
					
					if (board[i][j].revealed) //draw this if cell is revealed
					{
						switch (board[i][j].state)
						{
						case 0://solid - Rock
						{
							DrawTexture(Rock, board[i][j].posx, board[i][j].posy, RAYWHITE);
						}
						break;
						case 1:// negative - Monster
						{
							DrawTextureRec(Monster, Rectangle{ (float)((currentFrame + 1) * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ board[i][j].posx,board[i][j].posy }, WHITE);
						}
						break;
						case 2:// negative - Monster despawn 
						{
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
						}
						break;
						case 3://Medipack
						{
							DrawTexture(MediPack, board[i][j].posx, board[i][j].posy, RAYWHITE);
						}
						break;
						case 4://Shield
						{
							DrawTexture(Shield, board[i][j].posx, board[i][j].posy, RAYWHITE);
						}
						break;
						case 5://Gold
						{
							DrawTextureRec(Gold, Rectangle{ 0,0,(float)(Gold.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
						}
						break;
						case 6://Gold picked up (gold despawn = true setzen wenn player hit, dann wie bei monster)
						{
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
						}
						break;
						case 7://negative - hiddenTrap hidden
						{
							DrawTextureRec(Ladder, Rectangle{ 0,0,(float)(Trap.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
						}
						break;
						case 8://negative - hiddenTrap active
						{
							DrawTextureRec(Trap, Rectangle{ (float)(currentFrame * 60),0,(float)(Trap.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
						}
						break;
						case 9://Exit 
							DrawTexture(Ladder, board[i][j].posx, board[i][j].posy, RAYWHITE);
							break;

						default:// >=10 -> Empty field. Only floor layer visible plus Fog of War
							break;
						}
						

					}
					else //draw fog if Cell is not revealed
					{
						if (!board[i][j].revealed)
							DrawTexture(cloud[2], board[i][j].posx, board[i][j].posy, RAYWHITE);
						
					}

					//Sparkles as Hint for objects behind fog
					if (!board[i][j].revealed && board[i][j].state < 10 && board[i][j].state != 2 && board[i][j].state != 6 && board[i][j].state != 8)
					{
						DrawTextureRec(Sparkles, Rectangle{ (float)(currentFrame * 60),0,(float)(Sparkles.width / 4),60.0f }, board[i][j].GetCellVector(), WHITE);
						
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
			{
				DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
				if (IsKeyDown(KEY_SPACE))
				{
					DrawTexture(Light_down, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
				}
			}
			break;
			case 1://Up
			{
				DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),60,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
				if (IsKeyDown(KEY_SPACE))
				{
					DrawTexture(Light_up, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
				}
			}
			break;
			case 2://Right
			{
				DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),120,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
				if (IsKeyDown(KEY_SPACE))
				{
					DrawTexture(Light_right, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
				}
			}
			break;
			case 3://Left
			{
				DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),180,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
				if (IsKeyDown(KEY_SPACE))
				{
					DrawTexture(Light_left, player1.vDirection.x - 30, player1.vDirection.y - 30, WHITE);
				}
			}
			break;
			default:
				DrawTextureRec(Hero, Rectangle{ (float)(currentFrame * 60),0,(float)(Hero.width / 4),60.0f }, Vector2{ player1.posx,player1.posy }, WHITE);
				break;
			}


			//UI -Elements

			//Score
			DrawTextEx(customFont, (FormatText("Score : %06i", player1.score)), Vector2{ (16 * (float)(GetScreenWidth() / 32) + 10) ,(17 * (float)(GetScreenHeight() / 18) + 10) }, 20, 0, DARKPURPLE);

			//Gold
			DrawTextEx(customFont, (FormatText("Gold: %08i", player1.gold)), Vector2{ (8 * (float)(GetScreenWidth() / 32) + 10), (17 * (float)(GetScreenHeight() / 18) + 10) }, 20, 0, GOLD);

			//Current Level
			DrawTextEx(customFont, (FormatText("Level :  %1i", currentLevel)), Vector2{ (16 * (float)(GetScreenWidth() / 32) + 10) ,(17 * (float)(GetScreenHeight() / 18) + 30) }, 20, 0, DARKBROWN);

			//Health - Hearts and Shield
			//Hard - start 3 lifes no shield
			//medium 4 hearts no shield
			//easy 5 hearts shield
			switch (player1.lifes)
			{
			case 1:
			{
				if (player1.shield)
				{
					DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				}
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);
			}
			break;
			case 2:
			{
				if (player1.shield)
				{
					DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				}
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[1], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);
			}
			break;
			case 3:
			{
				if (player1.shield)
				{
					DrawTexture(ShieldIndicator, (8 * (GetScreenWidth() / 32) + 100), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				}
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 10), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 40), (17 * (GetScreenHeight() / 18) + 30), WHITE);
				DrawTexture(PowerUp[0], (8 * (GetScreenWidth() / 32) + 70), (17 * (GetScreenHeight() / 18) + 30), WHITE);
			}
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
		

			if (Pause)
			{
				if (isGameOver)
				{
					DrawTextEx(customFont, (FormatText("YOU DIED")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) }, 200, 0, GOLD);
					DrawTextEx(customFont, (FormatText("Press ENTER to proceed")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) + 250 }, 30, 0, GOLD);
				}
				if(!isGameOver&&!isVictory)
				{
					
					//Tutorial if Pause active
					DrawRectangle(120,120,600,840,BLACK);
					DrawRectangle(180, 180, 480, 720, DARKPURPLE);
					//Nebel
					DrawTexture(cloud[0],180,180,WHITE);
					DrawText("Reveal Darkness (SPACE-KEY).", 240, 200, 20, BLACK);
					//sparkles
					DrawTextureRec(Sparkles, Rectangle{60.0f,0,60.0f,60.0f }, Vector2{ 180.0f,240.0f }, WHITE);
					DrawText("Something hides in the Dark!", 240, 260, 20, BLACK);
					//Monster
					DrawTextureRec(Monster, Rectangle{ 60.0f,0,60.0f,60.0f }, Vector2{180.0f,300.0f }, WHITE);
					DrawText("Hurts!", 240, 320, 20, BLACK);
					//MediPacks
					DrawTexture(MediPack, 180, 360, WHITE);
					DrawText("Heals you.", 240, 380, 20, BLACK);
					//Shields
					DrawTexture(Shield, 180, 420, WHITE);
					DrawText("Protects from next sourge of damage.", 240, 440, 20, BLACK);
					//Gold
					DrawTextureRec(Gold, Rectangle{ 60.0f,0,60.0f,60.0f }, Vector2{ 180.0f,480.0f }, WHITE);
					DrawText("My precious, adds 100 Gold.", 240, 500, 20, BLACK);
					DrawText("Find in darkness or buy at market.", 240, 520, 20, BLACK);
					//Rocks
					DrawTexture(Rock, 180, 560, WHITE);
					DrawText("Can't be removed or passed.", 240, 580, 20, BLACK);
					//Exit
					DrawTexture(Ladder, 180, 620, WHITE);
					DrawText("Leads to Light or DOOM.", 240, 640, 20, BLACK);
					//Controls
					DrawText("Use WASD to move and SPACE to actvate", 200, 780, 20, BLACK);
					


					
					
					DrawTextEx(customFont, (FormatText("PAUSE")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) }, 200, 0, GOLD);
					DrawTextEx(customFont, (FormatText("Press ENTER for TITLESCREEN or P to resume ")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) + 250 }, 30, 0, GOLD);
				}
				if (isVictory)
				{					
					DrawTextEx(customFont, (FormatText("YOU WIN")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) }, 200, 0, GOLD);
					DrawTextEx(customFont, (FormatText("Press ENTER for next level")), Vector2{ (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) + 250 }, 30, 0, GOLD);
				}

			}
		}
		break;
		case STATISTICS:
		{
			//Background
			//for frametimer sleep animation
			if (currentFrame == 0)
			{
				DrawTexture(TitleSleep, 0, 0, WHITE);
			}
			else
			{
				DrawTexture(TitleScreen, 0, 0, WHITE);
			}

			//GameTitle Textfield
			DrawRectangle((GetScreenWidth() / 2), 60, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 70, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ 1020.0,120.0 }, 120, 0, BLACK);
			DrawText("Press SPACE to proceed", 1000, 990, 20, BLACK);

			//DialogeBox
			DrawRectangle((GetScreenWidth() / 2), 360, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 370, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			//Storyteller
			DrawText("Narrathor: ", 995, 375, 20, WHITE);
			if (languageGerman)
			{
				DrawText(TextSubtext(statisticsMessageGerman, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}
			else
			{
				DrawText(TextSubtext(statisticsMessage, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}

			//Highscore Textfield
			DrawRectangle((GetScreenWidth() / 2), ((GetScreenHeight() / 2) + 110), 500, 300, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), ((GetScreenHeight() / 2) + 130), 460, 260, DARKPURPLE);

			//Highscore Text
			DrawTextEx(customFont, (FormatText("Best Dreamer")), Vector2{ (float)((GetScreenWidth() / 2) + 50),(float)((GetScreenHeight() / 2) + 150) }, 30, 0, BLACK);
			DrawText(highscoreNameOld, ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 200), 50, BLACK);
			DrawText(FormatText("%8i", highscoreOld), ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 300), 50, BLACK);
			//Draw Stats Field
			DrawRectangle(1500, 650, 390, 300, BLACK);
			DrawRectangle(1510, 670, 370, 260, DARKPURPLE);

			//Draw Stats Text
			DrawTextEx(customFont, (FormatText("Levelstatistics")), Vector2{ 1520.0,680.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Fields revealed", fieldsRevealed)), Vector2{ 1550.0,720.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Medipacks collected", medipacksCollected)), Vector2{ 1550.0,750.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Shields collected", shieldsCollected)), Vector2{ 1550.0,780.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Gold collected", goldCollected)), Vector2{ 1550.0,810.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Monster found", monsterFound)), Vector2{ 1550.0,840.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Traps activated", trapsActivated)), Vector2{ 1550.0,870.0 }, 20, 0, BLACK);

			//SHOP
			if (player1.shield)
			{

			}
			else
			{
				if (player1.gold >= 500)
				{
					if (languageGerman)
					{
					DrawTextEx(customFont, (FormatText("Es is gefaehrlich da draussen kauf dir das:")), Vector2{ 1100.0,500.0 }, 20, 0, WHITE);
					DrawTextEx(customFont, (FormatText("Nur 500 Gold und es gehoert dir!")), Vector2{ 1100.0,530.0 }, 20, 0, WHITE);
					DrawTextEx(customFont, (FormatText("Duecke Enter zum kaufen")), Vector2{ 1100.0,560.0 }, 20, 0, WHITE);
					}
					else
					{
						DrawTextEx(customFont, (FormatText("It's dangarous to go alone, buy this:")), Vector2{ 1100.0,500.0 }, 20, 0, WHITE);
						DrawTextEx(customFont, (FormatText("Only 500 Gold and it's yours!")), Vector2{ 1100.0,530.0 }, 20, 0, WHITE);
						DrawTextEx(customFont, (FormatText("hit ENTER to buy")), Vector2{ 1100.0,560.0 }, 20, 0, WHITE);
					}
					

					DrawTexture(Shield, 1450, 520, WHITE);
				}
			}
		}
		break;
		case CREDITS:  
		{
			//Background
			DrawTexture(GrayBackground,0,0,WHITE);
			//Raylib-Logo
			DrawRectangle(60, 60, 256, 256, BLACK);
			DrawRectangle(76, 76, 224, 224, RAYWHITE);
			DrawText("raylib", 150,  240, 50, BLACK);
			
			//Credits Textfield
			DrawRectangle(((GetScreenWidth() / 32) * 6), 60, ((GetScreenWidth() / 32) * 25), (((GetScreenHeight() / 16) * 14) + 30), BLACK);
			DrawRectangle(((GetScreenWidth() / 32) * 7), 120, ((GetScreenWidth() / 32) * 23), (((GetScreenHeight() / 16) * 12)+45), DARKPURPLE);

			//Credit Texts
			
			DrawTextEx(customFont, (FormatText("****************************************************************************************************************************************************************************************************************************")), Vector2{ (float)(((GetScreenWidth() / 32) * 7)+30), (float)(((GetScreenWidth() / 32) * 2)+30) }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ (float)((GetScreenWidth() / 32)*8), 180.0f }, 60, 0, BLACK);
			DrawTextEx(customFont, (FormatText("When Dungeon Crawler meets Minesweeper")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 250.0f }, 30, 0, BLACK);
			DrawTextEx(customFont, (FormatText("by Andreas Lakus")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 290.0f }, 25, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Game Development Projekt - SRH Hochschule Heidelberg")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 320.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("This game has been created using raylib (www.raylib.com)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 380.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 410.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 440.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("****************************************************************************************************************************************************************************************************************************")), Vector2{ (float)(((GetScreenWidth() / 32) * 7) + 30), 470.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Used Software Products:")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 500.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Grafics made with:")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 530.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("GIMP v2.10.10 (www.gimp.org - openSource)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 560.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Sounds/Music made with:")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 590.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Ardour v 5.12 (www.ardour.org - openSource - self build)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 620.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("TAL NOIze M4K3R v 4.31 (www.tal-software.com - free to use)")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 650.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("****************************************************************************************************************************************************************************************************************************")), Vector2{ (float)(((GetScreenWidth() / 32) * 7) + 30), 690.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Special Tanks to:")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 720.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Ramon Santamaria")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 750.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("In this Project there are sound samples and font samples from the Raylib sample repository used.")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 780.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("Font: pixantiqua.ttf")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 810.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("sounds: boom ,buttonfx, coin, dash, die, fx_button, fx_leave, light_off, light_on, pause, sample_on, spring, start, win, wind_sound.")), Vector2{ (float)((GetScreenWidth() / 32) * 8), 840.0f }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("****************************************************************************************************************************************************************************************************************************")), Vector2{ (float)(((GetScreenWidth() / 32) * 7) + 30), (float)(((GetScreenWidth() / 32) * 15) + 30) }, 20, 0, BLACK);
		}
		break;
		case GAMEOVER:
		{
			//Background
			DrawTexture(GameOverScreen, 0, 0, WHITE);
			//GameTitle
			DrawRectangle((GetScreenWidth() / 2), 60, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 70, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			DrawTextEx(customFont, (FormatText("SWEET DREAMS")), Vector2{ 1020.0,120.0 }, 120, 0, BLACK);

			//Draw Stats Field
			DrawRectangle(1500, 650, 390, 300, BLACK);
			DrawRectangle(1510, 670, 370, 260, DARKPURPLE);

			//Draw Stats Text
			DrawTextEx(customFont, (FormatText("Levelstatistics")), Vector2{ 1520.0,680.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Fields revealed", fieldsRevealed)), Vector2{ 1550.0,720.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Medipacks collected", medipacksCollected)), Vector2{ 1550.0,750.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Shields collected", shieldsCollected)), Vector2{ 1550.0,780.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Gold collected", goldCollected)), Vector2{ 1550.0,810.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Monster found", monsterFound)), Vector2{ 1550.0,840.0 }, 20, 0, BLACK);
			DrawTextEx(customFont, (FormatText("%2i Traps activated", trapsActivated)), Vector2{ 1550.0,870.0 }, 20, 0, BLACK);
			//DialogeBox
			DrawRectangle((GetScreenWidth() / 2), 360, ((GetScreenWidth() / 2) - 20), 240, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), 370, ((GetScreenWidth() / 2) - 60), 220, DARKPURPLE);
			//Storyteller
			DrawText("Narrathor: ", 995, 375, 20, WHITE);
			if (languageGerman)
			{
				DrawText(TextSubtext(gameOverMessageGerman, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}
			else
			{
				DrawText(TextSubtext(gameOverMessage, 0, textSpeed / 10), 1000, 400, 20, WHITE);
				textSpeed += 4;
			}
			//Highscore Field
			DrawRectangle((GetScreenWidth() / 2), ((GetScreenHeight() / 2) + 110), 500, 300, BLACK);
			DrawRectangle(((GetScreenWidth() / 2) + 20), ((GetScreenHeight() / 2) + 130), 460, 260, DARKPURPLE);

			//Highscore Text
			DrawTextEx(customFont, (FormatText("HIGHSCORE")), Vector2{ (float)((GetScreenWidth() / 2) + 50),(float)((GetScreenHeight() / 2) + 150) }, 30, 0, BLACK);
			
			DrawText(highscoreNameOld, ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 200), 20, BLACK);
			DrawText(FormatText("%8i", highscoreOld), ((GetScreenWidth() / 2) + 50), ((GetScreenHeight() / 2) + 250), 20, BLACK);
			DrawText("Press SPACE to proceed", 1000, 990, 20, BLACK);
			//Check if new Highscore, then display input field
			if (player1.score > highscoreOld)
			{
				DrawRectangle(((GetScreenWidth() / 2) + 150), ((GetScreenHeight() / 2) + 300), 200, 60, BLACK);
				DrawRectangle(((GetScreenWidth() / 2) + 160), ((GetScreenHeight() / 2) + 310), 180, 40, DARKGRAY);
				DrawText("Press Enter to confirm input",1000,970,20,BLACK);
				DrawText("Press SPACE to proceed", 1000, 990, 20, BLACK);
				DrawText("No Space betweed letters allowed!", 1000, 1010, 20, BLACK);


				//Input Name for Highscore until max inputchars
				if (inputCounter < MAXINPUT)
				{
					// Draw underscore char
					if (inputCounter == 0)
					{
						DrawText(" Input Name ", ((GetScreenWidth() / 2) + 180), ((GetScreenHeight() / 2) + 320), 20, BLACK);
					}
					else
					{
						DrawTextEx(customFont, highscoreNameNew, Vector2{ (float)((GetScreenWidth() / 2) + 180), (float)((GetScreenHeight() / 2) + 320) }, 20, 0, BLACK);
					}
				}

			}
		}
		break;
		default:
		break;
		}

		EndDrawing();
		//----------------------------------------------------------------------------------		
	}
// De-Initialization
//--------------------------------------------------------------------------------------   

CloseWindow();
//--------------------------------------------------------------------------------------
return 0;
}

void initScreen()
{
	
	InitWindow(screenWidth, screenHeight, "Sweet Dreams");
	ToggleFullscreen();
	SetTargetFPS(60);
	InitAudioDevice();
	//SetWindowIcon();
	//SetWindowTitle();
	
}

void initCells()
{
	//Cell Array 
	int exitCounter = 0;
	int trapCounter = 0;

	for (int i = 0; i < COLUMNS; i++)
	{
		

		for (int j = 0; j < ROWS; j++)
		{
			

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
