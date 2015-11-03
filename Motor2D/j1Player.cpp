#include "p2Defs.h"
#include "j1App.h"
#include "j1Player.h"
#include "p2Log.h"
#include "Physics\ModulePhysics.h"
#include "j1Input.h"

j1Player::j1Player(j1App* app) : j1Module()
{
	name.create("player");
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config){

	init_player_values.folder.create(config.child_value("folder"));

	//Ball variables
	init_player_values.ball_src.create(config.child("ball").attribute("src").as_string());
	init_player_values.init_ball_x = config.child("ball").attribute("x").as_int();
	init_player_values.init_ball_y = config.child("ball").attribute("y").as_int();
	init_player_values.ball_radius = config.child("ball").attribute("radius").as_int();
	return true;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");
	//Ball

	ball.image = App->tex->Load(PATH(init_player_values.folder.GetString(), init_player_values.ball_src.GetString()));
	ball.body = App->physics->CreateCircle(init_player_values.init_ball_x, 
		init_player_values.init_ball_y, 
		init_player_values.ball_radius, 
		b2_dynamicBody, 0.1f, true);
	ball.body->listener = this;

	//Plunger
	plunger.image = App->tex->Load("data/textures/plunger.png");
	plunger_force = 0.0f;
	plunger.body = App->physics->CreateRectangle(477,945,33,33,true);
	plunger_axis = App->physics->CreateRectangle(477,945,10,10,false);
	App->physics->CreatePrismaticJoint(plunger.body, plunger_axis,false,0,100,true,100,100.0f,true);

	//Flippers
	/*int flipper1_points[16] = {
		1, 8,
		9, 1,
		73, 4,
		79, 9,
		79, 17,
		74, 21,
		10, 25,
		1, 18
	};*/

	//flipper1.image = App->tex->Load("data/textures/flipper.png");
	//flipper1.body = App->physics->CreatePolygon(141+500,827,flipper1_points,16,b2_dynamicBody,0.2f, 30.0f);
	//flipper1_axis = App->physics->CreateCircle(146+500, 844, 5, b2_staticBody);
	//flipper1_wheel = App->physics->CreateCircle(146 + 500, 844, 5, b2_staticBody);

	/*flipper1.revoluteJoint = 
		App->physics->CreateRevoluteJoint(flipper1.body, flipper1_axis,
		flipper1.body->GetPosition(), flipper1_axis->GetPosition(), true, 90, 70, -150, 100);*/

	//App->physics->CreateRevoluteJoint(flipper1.body, flipper1_wheel, 14, 17, 0, 0, 30, -30);


		//CreateRevoluteJoint(flipper1.body, flipper1_axis,false,0,0,-30,0,false,180, 0, 100, -150, true);



	int flipper1_points[16] = {
		1, 8,
		9, 1,
		73, 4,
		79, 9,
		79, 17,
		74, 21,
		10, 25,
		1, 18
	};
	flipper1.image = App->tex->Load("data/textures/flipper.png");
	flipper1.body = App->physics->AddBody({ 133, 830, 0, 0 }, flipper1_points, 16, b_dynamic);
	flipper1_wheel = App->physics->AddBody(133 + 14, 830 + 13, 10, b_static);
	App->physics->CreateRevoluteJoint(flipper1.body, flipper1_wheel, 14, 17, 0, 0, 30, -30);


	
	int flipper2_points[16] = {
		78, 12,
		73, 2,
		58, 0,
		5, 5,
		0, 11,
		5, 20,
		67, 24,
		77, 17
	};
/*
	flipper2.image = App->tex->Load("data/textures/flipper2.png");
	flipper2.body = App->physics->AddBody({300, 830, 0, 0 }, flipper2_points, 14, b_dynamic);
	flipper2_wheel = App->physics->AddBody(300 + 79-14, 830 + 13, 10, b_static);
	App->physics->CreateRevoluteJoint(flipper2.body, flipper2_wheel, 14, 17, 0, 0, -30, 30);
	*/
	flipper2.image = App->tex->Load("data/textures/flipper2.png");
	flipper2.body = App->physics->AddBody({ 314, 839, 0, 0 }, flipper2_points, 16, b_dynamic);
	flipper2_wheel = App->physics->AddBody(314, 839, 10, b_static);
	App->physics->CreateRevoluteJoint(flipper2.body, flipper2_wheel, 70, 13, 0, 0, 30, -30);

	
	/*
	flipper2.image = App->tex->Load("data/textures/flipper2.png");
	flipper2.body = App->physics->AddBody({ 300, 830, 0, 0 }, flipper2_points, 14, b_dynamic);
	flipper2_wheel = App->physics->AddBody(300 + 14, 830 + 17, 10, b_static);
	App->physics->CreateRevoluteJoint(flipper2.body, flipper2_wheel, 14, 17, 0, 0, 30, -30);

	*/


	return true;
}

void j1Player::CreateBall()
{

	ball.image = App->tex->Load(PATH(init_player_values.folder.GetString(), init_player_values.ball_src.GetString()));
	ball.body = App->physics->CreateCircle(init_player_values.init_ball_x,
		init_player_values.init_ball_y,
		init_player_values.ball_radius,
		b2_dynamicBody, 0.1f, true);
	ball.body->listener = this;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->tex->UnLoad(ball.image);
	//App->physics->DestroyBody(ball.body);

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		flipper1.body->Turn(-360);
		//flipper1.body->body->ApplyAngularImpulse(360, true);
		//flipper1.revoluteJoint;

		int a = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper2.body->Turn(360);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		plunger_force += 50.0f;
		plunger.body->Push(0, plunger_force);
	}
	else{
		plunger_force = 0.0f;
	}

	int x, y;

	ball.body->GetPosition(x, y);
	App->render->Blit(ball.image, x, y);//, ball.body->GetAngle());
	plunger.body->GetPosition(x, y);
	App->render->Blit(plunger.image, x, y);

	flipper1.body->GetPosition(x, y);
	App->render->Blit(flipper1.image, x/*132*/, y/*840*/, NULL, NULL, flipper1.body->GetRotation(),0,0);

	flipper2.body->GetPosition(x, y);
//<<<<<<< HEAD
	App->render->Blit(flipper2.image, x, y, NULL, NULL, flipper2.body->GetRotation(), 0, 0);
//=======
//	App->render->Blit(flipper2.image, x/*132*/, y/*840*/, //NULL, //NULL, flipper2.body->GetRotation(), 79, 0);
//>>>>>>> origin/Chains*/

	return true;
}