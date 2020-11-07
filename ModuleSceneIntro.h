#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

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
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* bumper1 = nullptr;
	PhysBody* bumper2 = nullptr;
	PhysBody* bumper3 = nullptr;
	PhysBody* bumper4 = nullptr;
	PhysBody* bumper5 = nullptr;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* stage = nullptr;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
