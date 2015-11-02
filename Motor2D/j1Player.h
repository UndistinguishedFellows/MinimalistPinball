#pragma once
#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Render.h"


class PhysBody;

struct Object
{
	SDL_Texture* image;
	PhysBody* body;

	Object() : image(NULL), body(NULL)
	{}
};

struct InitPlayerValues
{
	p2SString	folder;

	p2SString	ball_src;
	int			init_ball_x;
	int			init_ball_y;
	int			ball_radius;

};

class j1Player : public j1Module
{
public:
	j1Player(j1App* app);
	virtual ~j1Player();
	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:

	Object ball;
	InitPlayerValues init_player_values;
};