#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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
	int leftCorner[34] = {
	74, 264,
	62, 269,
	52, 258,
	50, 215,
	55, 158,
	69, 112,
	106, 80,
	126, 69,
	198, 73,
	209, 88,
	205, 103,
	192, 113,
	161, 117,
	131, 134,
	110, 166,
	92, 203,
	78, 252
	};
	obj1 = App->physics->CreateGround(0, 0, leftCorner, 34);
	int leftMedium[22] = {
	125, 453,
	71, 369,
	65, 350,
	65, 316,
	84, 298,
	107, 296,
	133, 297,
	258, 392,
	257, 422,
	146, 455,
	133, 455
	};
	obj2 = App->physics->CreateGround(0, 0, leftMedium, 22);
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
	obj4 = App->physics->CreateGround(0, 0, leftBot, 18);
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
	obj5 = App->physics->CreateGround(0, 0, rightBot, 24);
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

	//create circles that will be the bumpers

	bumper1 = App->physics->CreateCircleStatic(188+ 35, 178+ 35, 35);
	bumper2 = App->physics->CreateCircleStatic(302 + 35, 111+ 35, 35);
	bumper3 = App->physics->CreateCircleStatic(402+ 35, 152+ 35, 35);
	bumper4 = App->physics->CreateCircleStatic(302 + 35, 219+ 35, 35);
	bumper5 = App->physics->CreateCircleStatic(221+ 35, 274+ 35, 35);
	bumper1->listener = this;
	bumper2->listener = this;
	bumper3->listener = this;
	bumper4->listener = this;
	bumper5->listener = this;

	// Flipers
	
	int leftFliper[8] = {
	184, 894,
	198, 860,
	283, 907,
	278, 919
	};
	// leftFliperB = App->physics->CreatePolygon(184, 894, leftFliper, 14);





	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

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
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	//c = boxes.getFirst();

	//while (c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
	//	if (ray_on)
	//	{
	//		int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	//		if (hit >= 0)
	//			ray_hit = hit;
	//	}
	//	c = c->next;
	//}

	//c = ricks.getFirst();

	//while (c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == sensor) {
		App->audio->PlayFx(bonus_fx);
	}

	/*
	int x, y;

	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

