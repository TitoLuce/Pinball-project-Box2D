#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

struct Light {

	PhysBody* fLight1;
	PhysBody* fLight2;
	PhysBody* fLight3;
	PhysBody* fLight4;
	PhysBody* fLight5;
	PhysBody* fLight6;
	PhysBody* fLight7;
	PhysBody* fLight8;
	PhysBody* fLight9;
	PhysBody* fLight10;
	PhysBody* fLight11;
	PhysBody* fLight12;
	PhysBody* fLight13;
	PhysBody* fLight14;
	PhysBody* fLight15;
	PhysBody* fLight16;
	PhysBody* fLight17;
	PhysBody* fLight18;
	PhysBody* fLight19;
	PhysBody* fLight20;
	PhysBody* fLight21;
	PhysBody* fLight22;
	PhysBody* fLight23;
	
	bool light1 = false;
	bool light2 = false;
	bool light3 = false;
	bool light4 = false;
	bool light5 = false;
	bool light6 = false;
	bool light7 = false;
	bool light8 = false;
	bool light9 = false;
	bool light10 = false;
	bool light11 = false;
	bool light12 = false;
	bool light13 = false;
	bool light14 = false;
	bool light15 = false;
	bool light16 = false;
	bool light17 = false;
	bool light18 = false;
	bool light19 = false;
	bool light20 = false;
	bool light21 = false;
	bool light22 = false;
	bool light23 = false;
		
	int lightTimer1;
	int lightTimer2;
	int lightTimer3;
	int lightTimer4;
	int lightTimer5;
	int lightTimer6;
	int lightTimer7;
	int lightTimer8;
	int lightTimer9;
	int lightTimer10;
	int lightTimer11;
	int lightTimer12;
	int lightTimer13;
	int lightTimer14;

	SDL_Rect arrowLeft = {0,33,22,36};
	SDL_Rect arrowRight = {43,33,22,36};
	SDL_Rect arrowLeft1 = {0,15,13,16};
	SDL_Rect arrowRight1 = {56,15,13,16};
	SDL_Rect smally = { 28,57,13,13 };
	SDL_Rect superSmally = {31,15,6,7};
	SDL_Rect flatTop = { 0,0,20,13 };
	SDL_Rect flatTopG = { 24,0,19,13 };

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;

	PhysBody* background;

	// Objects
	PhysBody* table;

	PhysBody* obj1;
	PhysBody* obj2;
	PhysBody* obj3;
	PhysBody* obj4;
	PhysBody* obj5;
	PhysBody* obj6;
	PhysBody* obj7;

	// Flippers

	//PhysBody* leftFliperB;
	//PhysBody* rightFliperB;
	//PhysBody* topFliperB;

	// Top Blocker
	PhysBody* blockerSens;
	PhysBody* blockerSens2;
	PhysBody* blockerSens3;

	PhysBody* bigBlock;
	bool isBlockerTop;
	SDL_Texture* blockerTex = nullptr;

	SDL_Texture* holeTex = nullptr;
	bool isHoleActive;
	bool isBallinHole;
	PhysBody* hole;


	struct Flipper
	{
		PhysBody* polygon;
		PhysBody* rotor;
		SDL_Rect drawingRect;
		
	};

	Flipper* leftFlipper = new Flipper;
	SDL_Rect leftrect = {};
	Flipper* topFlipper = new Flipper;
	SDL_Rect toprect = {};
	Flipper* rightFlipper = new Flipper;
	SDL_Rect rightrect = {};


	//Lights stuff
	Light lights;
	

	p2List<Flipper*> flippers;


	PhysBody* bumper1 = nullptr;
	PhysBody* bumper2 = nullptr;
	PhysBody* bumper3 = nullptr;
	PhysBody* bumper4 = nullptr;
	PhysBody* bumper5 = nullptr;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* stage = nullptr;
	SDL_Texture* bumper = nullptr;
	SDL_Texture* title = nullptr;
	SDL_Texture* lightsTex = nullptr;
	SDL_Texture* flippersTex = nullptr;
	uint bonus_fx;
	uint ding_fx;
	p2Point<int> ray;
	bool ray_on;

	//Game and ball states
	bool once = true;
	bool musicOnce = true;
	enum GameState
	{
		NEW_GAME,
		PLAYING,
		BALL_LOST,
		GAME_OVER,
	};
	GameState gameState = GameState::NEW_GAME;

	bool isBallAlive;

	//Score system and font
	bool sensorBonus = false;
	bool perfectShotBonus = false;
	int timeCounter = 0;
	int font = -1;
	int score = 0;
	int highScore = 0;
	int previousScore = 0;
	int bonusScore;
	int balls;
	char scoreText[14] = { "\0" };
	char highScoreText[14] = { "\0" };
	char previousScoreText[14] = { "\0" };
	char ballsText[2]= { "\0" };

	//"Spring" animation
	SDL_Texture* springTexture;
	Animation spring;
	Animation* springPointer = &spring;
};


