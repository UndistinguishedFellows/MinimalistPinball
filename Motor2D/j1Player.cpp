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
	plunger.body = App->physics->CreateRectangle(476,950,35,35,true);
	plunger_axis = App->physics->CreateRectangle(476,950,10,10,false);
	App->physics->CreatePrismaticJoint(plunger.body, plunger_axis,false,0,100,true,100,2.0f,true);

	return true;
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

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		plunger_force += 2.0f;
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

	return true;
}