#pragma once
#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "j1Render.h"


class PhysBody;
class b2RevoluteJoint;

struct Object
{
	SDL_Texture* image;
	PhysBody* body;
	b2RevoluteJoint* revoluteJoint;


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
	void CreateBall();
public:

	Object ball;
	Object plunger;
	PhysBody* plunger_axis;
	Object flipper1;
	PhysBody* flipper1_axis;
	Object flipper2;
	PhysBody* flipper2_axis;
	float plunger_force;

	InitPlayerValues init_player_values;


	PhysBody* flipper1_wheel;
	PhysBody* flipper2_wheel;

};