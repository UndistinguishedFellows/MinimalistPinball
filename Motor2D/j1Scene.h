#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class Chain;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	

private:
	SDL_Texture* debug_tex;
	//p2List<p2List<int>*>* chains;
	p2List<Chain*>* chains;

	PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;
	PhysBody* lkicker;
	PhysBody* rkicker;
	PhysBody* lwall_act;
	PhysBody* rwall_act;
	PhysBody* lwall_des;
	PhysBody* rwall_des;

	SDL_Texture* mesa;
	SDL_Texture* bumper;
	SDL_Texture* kicker;
	SDL_Texture* kicker2;

	uint bumper1Sound;


	int bumper1Collision;
	int bumper2Collision;
	int bumper3Collision;
	int lkickerCollision;
	int rkickerCollision;
};

#endif // __j1SCENE_H__