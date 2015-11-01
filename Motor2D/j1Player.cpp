#include "p2Defs.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "Physics\ModulePhysics.h"
#include "j1Input.h"
#include "j1Render.h"

j1Player::j1Player(j1App* app) : j1Module()
{
	name.create("player");
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config){
	folder.create(config.child("folder").child_value());
	init_player_values = config;
	return true;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	ball.image = App->tex->Load(PATH(folder.GetString(), init_player_values.child("ball").attribute("src").as_string()));

	ball.body = App->physics->CreateCircle(init_player_values.child("ball").attribute("x").as_int(), init_player_values.child("ball").attribute("y").as_int(), init_player_values.child("ball").attribute("radius").as_int());
	ball.body->listener = this;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->tex->UnLoad(ball.image);
	App->physics->DestroyBody(ball.body);

	return true;
}

// Update: draw background
bool j1Player::Update()
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

	}

	int x, y;

	ball.body->GetPosition(x, y);
	App->render->Blit(ball.image, x, y, NULL, 1.0f);//, ball.body->GetAngle());

	return true;
}