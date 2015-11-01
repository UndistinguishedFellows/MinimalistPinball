#pragma once
#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Textures.h"

class PhysBody;

struct Object
{
	SDL_Texture* image;
	PhysBody* body;

	Object() : image(NULL), body(NULL)
	{}
};

class j1Player : public j1Module
{
public:
	j1Player(j1App* app);
	virtual ~j1Player();
	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update();
	bool CleanUp();

public:

	Object ball;
	p2SString folder;
	pugi::xml_node init_player_values;
};