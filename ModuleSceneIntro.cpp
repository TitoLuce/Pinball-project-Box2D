#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle  = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	//SDL_Texture Loading
	stage = App->textures->Load("pinball/Stage.png");
	circle = App->textures->Load("pinball/wheel.png");
	bumper = App->textures->Load("pinball/Bumper.png");
	title = App->textures->Load("pinball/Title.png");

	//SDL_Load Audio
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	//Chains
	int stage[156] = {
	1, 998,
	224, 997,
	224, 966,
	21, 873,
	20, 652,
	64, 609,
	65, 603,
	38, 574,
	94, 495,
	94, 482,
	75, 456,
	55, 427,
	45, 404,
	38, 381,
	32, 356,
	28, 331,
	24, 300,
	20, 270,
	18, 229,
	18, 191,
	21, 153,
	32, 127,
	43, 107,
	57, 90,
	69, 78,
	85, 67,
	105, 56,
	124, 48,
	145, 46,
	168, 45,
	197, 42,
	227, 41,
	276, 42,
	339, 44,
	391, 44,
	462, 46,
	498, 64,
	535, 84,
	560, 104,
	579, 120,
	595, 140,
	607, 160,
	616, 180,
	617, 205,
	617, 233,
	618, 918,
	595, 919,
	593, 191,
	574, 164,
	552, 140,
	523, 118,
	489, 94,
	465, 82,
	447, 82,
	436, 96,
	435, 131,
	508, 197,
	537, 223,
	557, 246,
	568, 264,
	571, 280,
	570, 380,
	562, 406,
	545, 427,
	524, 449,
	506, 472,
	505, 499,
	556, 570,
	534, 590,
	534, 610,
	573, 650,
	574, 877,
	375, 967,
	374, 998,
	624, 997,
	624, 0,
	0, 0,
	0, 990
	};
	table = App->physics->CreateGround(0, 0, stage, 156);
	int leftCorner[54] = {
		59, 268,
		51, 260,
		53, 171,
		58, 150,
		63, 134,
		69, 118,
		77, 108,
		86, 99,
		100, 88,
		113, 78,
		126, 74,
		200, 73,
		208, 81,
		208, 105,
		202, 110,
		166, 116,
		153, 120,
		138, 129,
		126, 140,
		115, 153,
		104, 174,
		96, 196,
		89, 215,
		85, 230,
		81, 250,
		72, 266,
		67, 268
	};
	obj1 = App->physics->CreateGround(0, 0, leftCorner, 54);
	int leftMedium[62] = {
		66, 314,
		75, 303,
		86, 298,
		101, 294,
		115, 294,
		129, 296,
		140, 300,
		149, 305,
		164, 314,
		185, 330,
		204, 347,
		221, 362,
		231, 371,
		241, 377,
		247, 380,
		257, 385,
		255, 390,
		255, 403,
		257, 417,
		254, 423,
		237, 429,
		208, 436,
		170, 446,
		142, 455,
		131, 455,
		118, 446,
		103, 424,
		84, 395,
		72, 373,
		66, 352,
		65, 320
	};
	obj2 = App->physics->CreateGround(0, 0, leftMedium, 62);
	int rightMedium[50] = {
	457, 429,
	458, 404,
	480, 375,
	499, 354,
	499, 342,
	492, 336,
	482, 336,
	468, 344,
	420, 394,
	405, 392,
	385, 378,
	383, 330,
	391, 306,
	408, 285,
	428, 275,
	487, 274,
	525, 287,
	536, 304,
	540, 336,
	538, 372,
	528, 396,
	511, 425,
	490, 440,
	475, 442,
	462, 432
	};
	obj3 = App->physics->CreateGround(0, 0, rightMedium, 50);
	int leftBot[18] = {
	108, 790,
	107, 684,
	112, 677,
	122, 678,
	208, 775,
	209, 810,
	199, 821,
	171, 821,
	113, 794
	};
	obj4 = App->physics->CreateGround(0, 0, leftBot, 18); //left triangle 
	int rightBot[24] = {
	387, 816,
	387, 791,
	472, 689,
	485, 688,
	489, 693,
	489, 795,
	485, 802,
	474, 803,
	468, 800,
	434, 815,
	411, 821,
	394, 820
	};
	obj5 = App->physics->CreateGround(0, 0, rightBot, 24); //right triangle
	int leftBase[26] = {
	52, 688,
	52, 668,
	66, 662,
	80, 669,
	81, 819,
	165, 850,
	186, 862,
	185, 875,
	175, 884,
	146, 883,
	93, 862,
	54, 838,
	53, 697
	};
	obj6 = App->physics->CreateGround(0, 0, leftBase, 26);
	int rightBase[26] = {
	411, 865,
	427, 853,
	518, 818,
	517, 687,
	517, 669,
	532, 662,
	547, 670,
	547, 688,
	546, 839,
	499, 864,
	454, 882,
	423, 884,
	414, 874
	};
	obj7 = App->physics->CreateGround(0, 0, rightBase, 26);

	obj4->listener = this;
	obj5->listener = this;



	//create circles that will be the bumpers

	bumper1 = App->physics->CreateCircleStatic(188 + 35, 178 + 35, 35);
	bumper2 = App->physics->CreateCircleStatic(302 + 35, 111 + 35, 35);
	bumper3 = App->physics->CreateCircleStatic(402 + 35, 152 + 35, 35);
	bumper4 = App->physics->CreateCircleStatic(302 + 35, 219 + 35, 35);
	bumper5 = App->physics->CreateCircleStatic(221 + 35, 274 + 35, 35);
	bumper1->listener = this;
	bumper2->listener = this;
	bumper3->listener = this;
	bumper4->listener = this;
	bumper5->listener = this;

	// Flipers
	
	b2Vec2 athing = { -0.6, 0 };
	b2Vec2 bthing = { 0, 0 };

	Flipper* leftFlipper = new Flipper;
	leftFlipper->rotor = App->physics->CreateCircleStatic(205, 885, 4);
	leftFlipper->polygon = App->physics->CreateRectangle(209, 920, 90, 26);
	App->physics->CreateRevoluteJoint(leftFlipper->polygon, athing, leftFlipper->rotor, bthing, 15.0f);
	flippers.add(leftFlipper);

	Flipper* topFlipper = new Flipper;
	topFlipper->rotor = App->physics->CreateCircleStatic(268, 405, 4);
	topFlipper->polygon = App->physics->CreateRectangle(270, 390, 60, 20);
	App->physics->CreateRevoluteJoint(topFlipper->polygon, athing, topFlipper->rotor, bthing, 15.0f);
	flippers.add(topFlipper);



	athing = { 0.6, 0 };


	Flipper* rightFlipper = new Flipper;
	rightFlipper->rotor = App->physics->CreateCircleStatic(395, 885, 4);
	rightFlipper->polygon = App->physics->CreateRectangle(209, 904, 90, 26);
	App->physics->CreateRevoluteJoint(rightFlipper->polygon, athing, rightFlipper->rotor, bthing, 15.0f);
	flippers.add(rightFlipper);



	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//Font
	font = App->fonts->Load("pinball/Font2.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ�123456789.:-+*/_!?0", 1);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Inputs========================================================
	if (gameState == GameState::PLAYING)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT))
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				p2List_item<Flipper*>* f = flippers.getFirst();
				while (f != NULL)
				{

					f->data->polygon->body->ApplyForce({ -100,0 }, { 0,0 }, true);

					f = f->next;
				}
			}

		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) /*&& !App->player->game_over*/) {
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				p2List_item<Flipper*>* f = flippers.getFirst();
				while (f != NULL)
				{

					f->data->polygon->body->ApplyForce({ 100,0 }, { 0,0 }, true);

					f = f->next;
				}
			}


		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ray_on = !ray_on;
			ray.x = App->input->GetMouseX();
			ray.y = App->input->GetMouseY();
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
			circles.getLast()->data->listener = this;
			circles.getLast()->data->body->SetBullet(true);
		}
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	App->renderer->Blit(stage, 0, 0);
	App->renderer->Blit(bumper, 188 , 178 );
	App->renderer->Blit(bumper, 302 , 111 );
	App->renderer->Blit(bumper, 402 , 152 );
	App->renderer->Blit(bumper, 302 , 219 );
	App->renderer->Blit(bumper, 221 , 274);

	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	
	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	if (gameState == GameState::NEW_GAME)
	{
		App->renderer->Blit(title, 0, 0, false);
		score = 0;
		if (App->input->GetKey(SDL_SCANCODE_RETURN))
		{
			gameState = GameState::PLAYING;
		}
	}

	//Score system & balls
	if (score > 9999999)
	{
		score = 9999999;
	}
	if (score > highScore)
	{
		highScore = score;
	}

	App->fonts->BlitText(208, 725, font, "SCORE:");
	sprintf_s(scoreText, 14, "%07d", score);
	App->fonts->BlitText(295, 725, font, scoreText);

	App->fonts->BlitText(240, 625, font, "HIGH SCORE");
	sprintf_s(highScoreText, 14, "%07d", highScore);
	App->fonts->BlitText(260, 645, font, highScoreText);

	App->fonts->BlitText(210, 675, font, "PREVIOUS SCORE");
	sprintf_s(previousScoreText, 14, "%7d", previousScore);
	App->fonts->BlitText(215, 695, font, previousScoreText);


	App->fonts->BlitText(25, 950, font, "BALLS LEFT:");
	sprintf_s(ballsText, 2, "%1d", balls);
	App->fonts->BlitText(185, 950, font, ballsText);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == sensor) {
		App->audio->PlayFx(bonus_fx);
	}



	if (bodyA == bumper1 || bodyA == bumper2 || bodyA == bumper3 || bodyA == bumper4 || bodyA == bumper5) {
		b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
		force *= 5;
		bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);

		App->scene_intro->score += 100;
	}

	 
	if (bodyA == obj4) {  //left triangle

		bodyB->body->ApplyLinearImpulse(b2Vec2(0.8, -2.5), bodyB->body->GetWorldCenter(), true);
		App->scene_intro->score += 100;
		
		//do left triangle animation
		//do left triangle sound

	}

	if (bodyA == obj5) {  //right triangle

		bodyB->body->ApplyLinearImpulse(b2Vec2(-0.8, -2.5), bodyB->body->GetWorldCenter(), true);
		App->scene_intro->score += 100;

		//do right triangle animation
		//do right triangle sound

	}


}
