#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

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
	bool light1 = false;
	bool light2 = false;
	bool light3 = false;
	bool light4 = false;
	bool light5 = false;
	bool light6 = false;
	bool light7 = false;
	bool light8 = false;
	int lightTimer1;
	SDL_Rect flight_rect;
	SDL_Rect light_on_rect;
	bool light_time = false;
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

	struct Flipper
	{
		PhysBody* polygon;
		PhysBody* rotor;
		SDL_Rect drawingRect;
		
	};

	Flipper* leftFlipper = new Flipper;
	Flipper* topFlipper = new Flipper;
	Flipper* rightFlipper = new Flipper;


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
	SDL_Texture* flipersTex = nullptr;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	//Game and ball states
	enum GameState
	{
		NEW_GAME,
		PLAYING,
		BALL_LOST,
		GAME_OVER,
	};
	GameState gameState = GameState::NEW_GAME;

	bool isBallAlive = false;

	//Score system and font
	int font = -1;
	int score = 0;
	int highScore = 0;
	int previousScore = 0;
	int balls = 0;
	char scoreText[14] = { "\0" };
	char highScoreText[14] = { "\0" };
	char previousScoreText[14] = { "\0" };
	char ballsText[2]= { "\0" };

};


