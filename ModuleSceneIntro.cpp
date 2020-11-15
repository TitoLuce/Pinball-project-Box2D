#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "Animation.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle  = NULL;
	ray_on = false;
	sensed = false;
	isBlockerTop = false;
	isHoleActive = false;
	isBallinHole = false;


	//Normal animation
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			spring.PushBack({ j * 41,i*77,39,75 });
		}
	}
	for (int j = 0; j < 7; j++)
	{
		spring.PushBack({ j * 41, 462,39,75 });
	}
	//Stays on final frame a bit to give the player a small margin to hit the maximum value
	for (int a = 0; a < 2; a++)
	{
		spring.PushBack({ 246, 462,39,75 });
	}
	//Inverted animation
	for (int j = 0; j < 7; j++)
	{
		spring.PushBack({ 246 - j * 41, 462,39,75 });
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			spring.PushBack({ 328 - j * 41,385 - i * 77,39,75 });
		}
	}
	spring.SetSpeed(0.5f);
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	isBallAlive = true;

	bonusScore = 0;
	hole = App->physics->CreateRectangleSensor(482, 350, 15, 15);
	hole->listener = this;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	//SDL_Texture Loading
	stage = App->textures->Load("pinball/Stage.png");
	circle = App->textures->Load("pinball/wheel.png");
	bumper = App->textures->Load("pinball/Bumper.png");
	title = App->textures->Load("pinball/Title.png");
	lightsTex = App->textures->Load("pinball/Sensors.png");
	flippersTex = App->textures->Load("pinball/Flipers.png");
	blockerTex = App->textures->Load("pinball/BigBlocker.png");
	holeTex = App->textures->Load("pinball/BonusHole.png");

	//SDL_Load Audio
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	ding_fx = App->audio->LoadFx("pinball/Ding.wav");
	spring_fx = App->audio->LoadFx("pinball/Muelle.ogg");
	gameOver_fx = App->audio->LoadFx("pinball/GameOver.wav");
	

	//Blocker
	
	blockerSens = App->physics->CreateRectangleSensor(316,78,20,40);
	blockerSens2 = App->physics->CreateRectangleSensor(372, 115, 5, 5);
	blockerSens3 = App->physics->CreateRectangleSensor(390, 159, 25, 5);
	blockerSens->listener = this;
	blockerSens2->listener = this;
	blockerSens3->listener = this;


	//Lights
	lights.fLight1 = App->physics->CreateRectangleSensor(146, 60, 15, 15);
	lights.fLight2 = App->physics->CreateRectangleSensor(78, 94, 15, 15);
	lights.fLight3 = App->physics->CreateRectangleSensor(56, 118, 15, 15);
	lights.fLight4 = App->physics->CreateRectangleSensor(44, 148, 15, 15);
	lights.fLight5 = App->physics->CreateRectangleSensor(36, 180, 15, 15);
	lights.fLight6 = App->physics->CreateRectangleSensor(36, 210, 15, 15);
	lights.fLight7 = App->physics->CreateRectangleSensor(64, 400, 15, 20); // <- arrow left start
	lights.fLight8 = App->physics->CreateRectangleSensor(81, 430, 12, 12);
	lights.fLight9 = App->physics->CreateRectangleSensor(95, 452, 10, 10);
	lights.fLight10 = App->physics->CreateRectangleSensor(112, 474, 10, 10);
	lights.fLight11 = App->physics->CreateRectangleSensor(544, 398, 15, 20); // <- arrow right start
	lights.fLight12 = App->physics->CreateRectangleSensor(528, 428, 12, 12);
	lights.fLight13 = App->physics->CreateRectangleSensor(512, 450, 12, 12);
	lights.fLight14 = App->physics->CreateRectangleSensor(496, 469, 12, 12);
	lights.fLight15 = App->physics->CreateRectangleSensor(182, 452, 15, 15); // <- top three start
	lights.fLight16 = App->physics->CreateRectangleSensor(214, 447, 15, 15);
	lights.fLight17 = App->physics->CreateRectangleSensor(243, 441, 15, 15);
	lights.fLight18 = App->physics->CreateRectangleSensor(96, 508, 15, 15); // <- left three start
	lights.fLight19 = App->physics->CreateRectangleSensor(80, 526, 15, 15);
	lights.fLight20 = App->physics->CreateRectangleSensor(64, 549, 15, 15);
	lights.fLight21 = App->physics->CreateRectangleSensor(500, 507, 15, 15); // <- right three start
	lights.fLight22 = App->physics->CreateRectangleSensor(514, 527, 15, 15);
	lights.fLight23 = App->physics->CreateRectangleSensor(534, 549, 15, 15);

	lights.fLight1->listener = this;
	lights.fLight2->listener = this;
	lights.fLight3->listener = this;
	lights.fLight4->listener = this;
	lights.fLight5->listener = this;
	lights.fLight6->listener = this;
	lights.fLight7->listener = this;
	lights.fLight8->listener = this;
	lights.fLight9->listener = this;
	lights.fLight10->listener = this;
	lights.fLight11->listener = this;
	lights.fLight12->listener = this;
	lights.fLight13->listener = this;
	lights.fLight14->listener = this;
	lights.fLight15->listener = this;
	lights.fLight16->listener = this;
	lights.fLight17->listener = this;
	lights.fLight18->listener = this;
	lights.fLight19->listener = this;
	lights.fLight20->listener = this;
	lights.fLight21->listener = this;
	lights.fLight22->listener = this;
	lights.fLight23->listener = this;

	lights.lightTimer1 = 0;
	lights.lightTimer2 = 0;
	lights.lightTimer3 = 0;
	lights.lightTimer4 = 0;
	lights.lightTimer5 = 0;
	lights.lightTimer6 = 0;
	lights.lightTimer7 = 0;
	lights.lightTimer8 = 0;
	lights.lightTimer9 = 0;
	lights.lightTimer10 = 0;
	lights.lightTimer11 = 0;
	lights.lightTimer12 = 0;
	lights.lightTimer13 = 0;
	lights.lightTimer14 = 0;

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

	// Flippers
	
	b2Vec2 athing = { -0.6, 0 };
	b2Vec2 bthing = { 0, 0 };

	
	leftFlipper->rotor = App->physics->CreateCircleStatic(205, 885, 4);
	leftFlipper->polygon = App->physics->CreateRectangle(209, 920, 90, 26);
	App->physics->CreateRevoluteJoint(leftFlipper->polygon, athing, leftFlipper->rotor, bthing, 15.0f);
	leftFlipper->drawingRect = { 0, 0, 89, 32 };
	flippers.add(leftFlipper);
	
	
	topFlipper->rotor = App->physics->CreateCircleStatic(268, 405, 4);
	topFlipper->polygon = App->physics->CreateRectangle(270, 390, 60, 20);
	App->physics->CreateRevoluteJoint(topFlipper->polygon, athing, topFlipper->rotor, bthing, 15.0f);
	topFlipper->drawingRect = { 0,46,45,15 };
	flippers.add(topFlipper);

	athing = { 0.6, 0 };

	
	rightFlipper->rotor = App->physics->CreateCircleStatic(395, 885, 4);
	rightFlipper->polygon = App->physics->CreateRectangle(209, 904, 90, 26);
	App->physics->CreateRevoluteJoint(rightFlipper->polygon, athing, rightFlipper->rotor, bthing, 15.0f);
	rightFlipper->drawingRect = { 112,0,89,32 };
	flippers.add(rightFlipper);
	



	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 10);
	sensor->listener = this;

	//Font
	font = App->fonts->Load("pinball/Font2.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZÑ123456789.:-+*/_!?0", 1);

	//"Spring"
	springTexture = App->textures->Load("pinball/ChargeBar.png");

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

	if (balls < 0 && gameState != GAME_OVER)
	{
		balls = 0;
		gameState = GAME_OVER;
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

	//Game state stansition maintenance
	if (gameState == GameState::NEW_GAME)
	{
		onceTwo = true;
		App->renderer->Blit(title, 0, 0, false);
		previousScore = score;
		if (once)
		{
			score = 0;
			once = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN))
		{
			once = true;
			balls = 3;
			gameState = GameState::PLAYING;
		}
	}

	if (gameState == GameState::PLAYING)
	{
		if (once)
		{
			score = 0;
			circles.add(App->physics->CreateCircle(596, 894, 10));
			circles.getLast()->data->listener = this;
			circles.getLast()->data->body->SetBullet(true);

			if (musicOnce) 
			{
				App->audio->PlayMusic("pinball/Off Pepper Steak (Extended).ogg");
				Mix_VolumeMusic(50);
				musicOnce = false;
			}
			once = false;
		}
	}

	if (gameState == GameState::GAME_OVER)
	{
		balls = 0;
		if (onceTwo)
		{
			App->audio->PlayFx(gameOver_fx, 20);
			onceTwo = false;
		}
		App->fonts->BlitText(230, 525, font, "GAME OVER");
		 App->fonts->BlitText(170, 545, font, "PRESS UP TO RESTART");
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			once = true;
			gameState = GameState::NEW_GAME;
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
	sprintf_s(previousScoreText, 14, "%07d", previousScore);
	App->fonts->BlitText(260, 695, font, previousScoreText);


	App->fonts->BlitText(25, 950, font, "BALLS LEFT:");
	sprintf_s(ballsText, 2, "%1d", balls);
	App->fonts->BlitText(185, 950, font, ballsText);

	if (sensorBonus)
	{
		App->fonts->BlitText(170, 525, font, "SENSOR BONUS +15000");
		if (timeCounter < 100)
		{
			timeCounter++;
		}
		else {
			sensorBonus = false;
			timeCounter = 0;
		}
	}

	if (perfectShotBonus)
	{
		App->fonts->BlitText(140, 525, font, "PERFECT SHOT BONUS +5000");
		if (timeCounter < 100)
		{
			timeCounter++;
		}
		else {
			perfectShotBonus = false;
			timeCounter = 0;
		}
	}


	//flippers blitters

	p2List_item<Flipper*>* f = flippers.getFirst();
	while (f != NULL)
	{
		int x, y;
		f->data->polygon->GetPosition(x, y);
		App->renderer->Blit(flippersTex, x, y, &f->data->drawingRect, 1.0f, f->data->polygon->GetRotation());
		f = f->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->physics->debug)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->body->SetBullet(true);
	}



	// Lights Logic ----------------------------------------------------------------

	if (lights.light1) {
		App->renderer->Blit(lightsTex, 147-10, 61-6, &lights.flatTop);
		lights.lightTimer1++;
		if (lights.lightTimer1 == 50)
		{
			lights.light1 = false;
			lights.lightTimer1 = 0;
		}
	}
	
	if (lights.light2) {
		App->renderer->Blit(lightsTex, 78 - 10, 95 - 6, &lights.flatTop);
		lights.lightTimer2++;
		if (lights.lightTimer2 == 50)
		{
			lights.light2 = false;
			lights.lightTimer2 = 0;
		}
	}

	if (lights.light3) {
		App->renderer->Blit(lightsTex, 56 - 10, 118 - 6, &lights.flatTop);
		lights.lightTimer3++;
		if (lights.lightTimer3 == 50)
		{
			lights.light3 = false;
			lights.lightTimer3 = 0;
		}
	}

	if (lights.light4) {
		App->renderer->Blit(lightsTex, 44 - 10, 148 - 6, &lights.flatTop);
		lights.lightTimer4++;
		if (lights.lightTimer4 == 50)
		{
			lights.light4 = false;
			lights.lightTimer4 = 0;
		}
	}

	if (lights.light5) {
		App->renderer->Blit(lightsTex, 36 - 10, 180 - 6, &lights.flatTopG);
		lights.lightTimer5++;
		if (lights.lightTimer5 == 50)
		{
			lights.light5 = false;
			lights.lightTimer5 = 0;
		}
	}

	if (lights.light6) {
		App->renderer->Blit(lightsTex, 38 - 10, 211 - 6, &lights.flatTop);
		lights.lightTimer6++;
		if (lights.lightTimer6 == 50)
		{
			lights.light6 = false;
			lights.lightTimer6 = 0;
		}
	}

	if (lights.light7) {
		App->renderer->Blit(lightsTex, 66 - 10, 389 - 6, &lights.arrowLeft);
		lights.lightTimer7++;
		if (lights.lightTimer7 == 50)
		{
			lights.light7 = false;
			lights.lightTimer7 = 0;
		}
	}

	if (lights.light8) {
		App->renderer->Blit(lightsTex, 81 - 6, 427 - 6, &lights.arrowLeft1);
		lights.lightTimer8++;
		if (lights.lightTimer8 == 50)
		{
			lights.light8 = false;
			lights.lightTimer8 = 0;
		}
	}

	if (lights.light9) {
		App->renderer->Blit(lightsTex, 95 - 5, 452 - 5, &lights.smally);
		lights.lightTimer9++;
		if (lights.lightTimer9 == 50)
		{
			lights.light9 = false;
			lights.lightTimer9 = 0;
		}
	}

	if (lights.light10) {
		App->renderer->Blit(lightsTex, 114 - 5, 475 - 5, &lights.superSmally);
		lights.lightTimer10++;
		if (lights.lightTimer10 == 50)
		{
			lights.light10 = false;
			lights.lightTimer10 = 0;
		}
	}

	if (lights.light11) {
		App->renderer->Blit(lightsTex, 542 - 10, 396 - 15, &lights.arrowRight);
		lights.lightTimer11++;
		if (lights.lightTimer11 == 50)
		{
			lights.light11 = false;
			lights.lightTimer11 = 0;
		}
	}

	if (lights.light12) {
		App->renderer->Blit(lightsTex, 531 - 6, 427 - 6, &lights.arrowRight1);
		lights.lightTimer12++;
		if (lights.lightTimer12 == 50)
		{
			lights.light12 = false;
			lights.lightTimer12 = 0;
		}
	}

	if (lights.light13) {
		App->renderer->Blit(lightsTex, 515 - 6, 452 - 6, &lights.smally);
		lights.lightTimer13++;
		if (lights.lightTimer13 == 50)
		{
			lights.light13 = false;
			lights.lightTimer13 = 0;
		}
	}

	if (lights.light14) {
		App->renderer->Blit(lightsTex, 503 - 6, 476 - 6, &lights.superSmally);
		lights.lightTimer14++;
		if (lights.lightTimer14 == 50)
		{
			lights.light14 = false;
			lights.lightTimer14 = 0;
		}
	}

	if (lights.light15) {
		App->renderer->Blit(lightsTex, 185 - 6, 455 - 6, &lights.smally);

	}

	if (lights.light16) {
		App->renderer->Blit(lightsTex, 218 - 6, 446 - 6, &lights.smally);
	}

	if (lights.light17) {
		App->renderer->Blit(lightsTex, 246 - 6, 438 - 6, &lights.smally);
	}

	if (lights.light18) {
		App->renderer->Blit(lightsTex, 100 - 6, 514 - 6, &lights.smally);
	}

	if (lights.light19) {
		App->renderer->Blit(lightsTex, 84 - 6, 534 - 6, &lights.smally);
	}

	if (lights.light20) {
		App->renderer->Blit(lightsTex, 69 - 6, 552 - 6, &lights.smally);
	}

	if (lights.light21) {
		App->renderer->Blit(lightsTex, 499 - 6, 511 - 6, &lights.smally);
	}

	if (lights.light22) {
		App->renderer->Blit(lightsTex, 515 - 6, 533 - 6, &lights.smally);
	}

	if (lights.light23) {
		App->renderer->Blit(lightsTex, 530 - 6, 552 - 6, &lights.smally);
	}

	// Blocker top logic
	if (isBlockerTop == true)
	{
		int BigBlockerP[28] = {
126, 106,
96, 75,
74, 55,
47, 39,
24, 27,
0, 19,
0, 0,
38, 0,
81, 15,
119, 32,
153, 56,
143, 58,
127, 73,
127, 102
		};
		if (bigBlock == nullptr)
		{
			bigBlock = App->physics->CreateGround(338, 46, BigBlockerP, 28);
		}
		App->renderer->Blit(blockerTex, 342 - 10, 66 - 20, NULL);
	}


	// Special combo logic
	if (lights.light15 == true && lights.light16 == true && lights.light17 == true &&
		lights.light18 == true && lights.light19 == true && lights.light20 == true &&
		lights.light21 == true && lights.light22 == true && lights.light23 == true)
	{
		isHoleActive = true;
		App->renderer->Blit(holeTex, 472, 338, NULL);
	}

	if (isHoleActive == true && isBallinHole == true)
	{
		if (bonusScore <= 2)
		{
			App->scene_intro->score += 15000;
			bonusScore++;
		}

		lights.light15 = false;
		lights.light16 = false;
		lights.light17 = false;
		lights.light18 = false;
		lights.light19 = false;
		lights.light20 = false;
		lights.light21 = false;
		lights.light22 = false;
		lights.light23 = false;
		isHoleActive = false;
		isBallinHole = false;
		bonusScore = 0;
		sensorBonus = true;
	}
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	//Default "spring" state
	springPointer = &spring;
	App->renderer->Blit(springTexture, SCREEN_WIDTH - 39, SCREEN_HEIGHT - 75, &springPointer->GetCurrentFrame(), false);

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
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) /*&& !App->player->game_over*/)
		{
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

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			springPointer->Update();
			App->renderer->Blit(springTexture, SCREEN_WIDTH - 39, SCREEN_HEIGHT - 75, &springPointer->GetCurrentFrame(), false);
		}
		else { spring.Reset(); } //Key release


	}

	if (!isBallAlive)
	{
		isBallAlive = true;
		
		p2List_item<PhysBody*>* c = circles.getFirst();
		while (c != NULL)
		{
			c->data->body->GetWorld()->DestroyBody(c->data->body);
			c = c->next;
		}
		circles.clear();
		
		if (bigBlock != nullptr) {
			bigBlock->body->GetWorld()->DestroyBody(bigBlock->body);
			bigBlock = nullptr;
		}

		balls--;
		if (balls >= 0)
		{
			circles.add(App->physics->CreateCircle(596, 894, 10));
			circles.getLast()->data->listener = (Module*)App->player;
		}
	}

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == sensor && gameState != GameState::BALL_LOST)
	{
		isBallAlive = false;
		isBlockerTop = false;
	}



	if (bodyA == bumper1 || bodyA == bumper2 || bodyA == bumper3 || bodyA == bumper4 || bodyA == bumper5) {
		b2Vec2 force(bodyB->body->GetWorldCenter() - bodyA->body->GetWorldCenter());
		force *= 5;
		bodyB->body->ApplyLinearImpulse(force, bodyB->body->GetWorldCenter(), true);
		App->audio->PlayFx(bonus_fx,128);
		App->scene_intro->score += 100;
	}

	 
	if (bodyA == obj4) {  //left triangle

		bodyB->body->ApplyLinearImpulse(b2Vec2(0.8, -2.5), bodyB->body->GetWorldCenter(), true);
		App->scene_intro->score += 100;
		
		//do left triangle animation
		//do left triangle sound
		App->audio->PlayFx(bonus_fx,128); //provisional

	}

	if (bodyA == obj5) {  //right triangle

		bodyB->body->ApplyLinearImpulse(b2Vec2(-0.8, -2.5), bodyB->body->GetWorldCenter(), true);
		App->scene_intro->score += 100;

		//do right triangle animation
		//do right triangle sound
		App->audio->PlayFx(bonus_fx,128); //provisional

	}

	// Lights Collisions ------------------------------------------------------------
	if (bodyA == lights.fLight1)
	{
		lights.light1 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight2)
	{
		lights.light2 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight3)
	{
		lights.light3 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight4)
	{
		lights.light4 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight5)
	{
		lights.light5 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight6)
	{
		lights.light6 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight7)
	{
		lights.light7 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight8)
	{
		lights.light8 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight9)
	{
		lights.light9 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight10)
	{
		lights.light10 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight11)
	{
		lights.light11 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight12)
	{
		lights.light12 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight13)
	{
		lights.light13 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight14)
	{
		lights.light14 = true;
		App->scene_intro->score += 10;

	}
	if (bodyA == lights.fLight15)
	{
		lights.light15 = true;
		App->audio->PlayFx(ding_fx,20);
	}
	if (bodyA == lights.fLight16)
	{
		lights.light16 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight17)
	{
		lights.light17 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight18)
	{
		lights.light18 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight19)
	{
		lights.light19 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight20)
	{
		lights.light20 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight21)
	{
		lights.light21 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight22)
	{
		lights.light22 = true;
		App->audio->PlayFx(ding_fx, 20);
	}
	if (bodyA == lights.fLight23)
	{
		lights.light23 = true;
		App->audio->PlayFx(ding_fx, 20);
	}

	// for the top blocker
	if (bodyA == blockerSens || bodyA == blockerSens2 || bodyA == blockerSens3)
	{
		isBlockerTop = true;
	}

	if (bodyA == hole && isHoleActive == true)
	{
		isBallinHole = true;
	}

}
