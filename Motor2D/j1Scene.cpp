#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "Physics\ModulePhysics.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	chains = new p2List<Chain*>();
}

// Destructor
j1Scene::~j1Scene()
{
	
}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	App->render->camera.x = App->render->camera.y = 0;

	for (pugi::xml_node iterator = config.child("chain"); 
		iterator != NULL; 
		iterator = iterator.next_sibling())
	{ 
		
		LOG("-----------------------------------Chain Childs: %d", 
			App->fs->getNumberOfNodes(iterator));
		
		Chain* chain = new Chain();
		p2List<int>* points = new p2List<int>();
		chain->points = points;
		chains->add(chain);
		chains->end->data->restitution = iterator.attribute("restitution").as_float();

		for (pugi::xml_node pointIterator = iterator.child("point");
			pointIterator != NULL;
			pointIterator = pointIterator.next_sibling())
		{
			chains->end->data->points->add(pointIterator.attribute("x").as_int());
			chains->end->data->points->add(pointIterator.attribute("y").as_int());
		}
	}
	
	

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	for (p2List_item<Chain*>* iterator = chains->start; 
		iterator != NULL; 
		iterator = iterator->next)
	{
		App->physics->CreateChain(0, 0, 
			iterator->data->points, 
			iterator->data->points->count(), b2_staticBody, 
			iterator->data->restitution);
	}
	
	//TODO: Put this to xmlConfig
	int radius = 27;
	bumper1 = App->physics->CreateCircle(281, 252, radius, b2_staticBody, 1.2f);
	bumper2 = App->physics->CreateCircle(169, 252, radius, b2_staticBody, 1.2f);
	bumper3 = App->physics->CreateCircle(224, 338, radius, b2_staticBody, 1.2f);
	lkicker = App->physics->CreateRectangle(115, 708, 10, 130,false, -19,2.0f);
	rkicker = App->physics->CreateRectangle(340, 708, 10, 130, false, 19, 2.0f);
	lwall_act = App->physics->CreateRectangleSensor(127, 93, 10, 130, 19);
	lwall_des = App->physics->CreateRectangleSensor(73, 121, 10, 130, -19);
	rwall_act = App->physics->CreateRectangleSensor(350, 68, 10, 130, -19);
	rwall_des = App->physics->CreateRectangleSensor(430, 130, 10, 130, 19);
	
	bumper1->listener = this;
	bumper2->listener = this;
	bumper3->listener = this;
	lkicker->listener = this;
	rkicker->listener = this;
	lwall_act->listener = this;
	lwall_des->listener = this;
	rwall_act->listener = this;
	rwall_des->listener = this;

	mesa = App->tex->Load("data/textures/mesa_vacia.png");
	bumper = App->tex->Load("data/textures/bumper.png");
	kicker = App->tex->Load("data/textures/kicker.png");
	kicker2 = App->tex->Load("data/textures/kicker2.png");

	bumper1Sound = App->audio->LoadFx("data/audio/fx/bumper.wav");


	bumper1Collision = 0;
	bumper2Collision = 0;
	bumper3Collision = 0;
	lkickerCollision = 0;
	rkickerCollision = 0;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15.0f, b2_dynamicBody, 0.1f, true);

	int x;

	//App->win->SetTitle("(%d,%d)");
	

	p2SString title("Mouse: (%d,%d)", 
		App->input->GetMouseX(), App->input->GetMouseY());

	App->win->SetTitle(title.GetString());

	App->render->Blit(mesa, 0, 4);

	if (bumper1Collision >= 1 && bumper1Collision <= 10)
	{
		App->render->Blit(bumper, 280 - 27, 251 - 27);
		bumper1Collision++;
	}
	else
		bumper1Collision = 0;
	if (bumper2Collision >= 1 && bumper2Collision <= 10)
	{
		App->render->Blit(bumper, 170 - 27, 251 - 27);
		bumper2Collision++;
	}
	else
		bumper2Collision = 0;
	if (bumper3Collision >= 1 && bumper3Collision <= 10)
	{
		App->render->Blit(bumper, 223 - 27, 336 - 27);
		bumper3Collision++;
	}
	else
		bumper3Collision = 0;
	if (lkickerCollision >= 1 && lkickerCollision <= 10)
	{
		App->render->Blit(kicker, 77, 643);
		lkickerCollision++;
	}
	else
		lkickerCollision = 0;
	if (rkickerCollision >= 1 && rkickerCollision <= 10)
	{
		App->render->Blit(kicker2, 315, 643);
		rkickerCollision++;
	}
	else
		rkickerCollision = 0;

	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	return ret;
}

void j1Scene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == bumper1 || bodyB == bumper1)
	{		
		bumper1Collision = 1;
		App->audio->PlayFx(bumper1Sound);
	}
	if (bodyA == bumper2 || bodyB == bumper2)
	{
		bumper2Collision = 1;
		App->audio->PlayFx(bumper1Sound);
	}
	if (bodyA == bumper3 || bodyB == bumper3)
	{
		bumper3Collision = 1;
		App->audio->PlayFx(bumper1Sound);
	}
	if (bodyA == lkicker || bodyB == lkicker)
	{
		lkickerCollision = 1;
		App->audio->PlayFx(bumper1Sound);
	}
	if (bodyA == rkicker || bodyB == rkicker)
	{
		rkickerCollision = 1;
		App->audio->PlayFx(bumper1Sound);
	}
	if (bodyA == lwall_act || bodyB == lwall_act)
	{

	}
	if (bodyA == lwall_des || bodyB == lwall_des)
	{

	}
	if (bodyA == rwall_act || bodyB == rwall_act)
	{

	}
	if (bodyA == rwall_des || bodyB == rwall_des)
	{

	}

}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	RELEASE(bumper1);
	RELEASE(bumper2);
	RELEASE(bumper3);
	return true;
}
