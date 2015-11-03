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
{}

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
	
	int radius = 30;

	App->physics->CreateCircle(281, 248, radius, b2_staticBody, 1.5f);
	App->physics->CreateCircle(169, 248, radius, b2_staticBody, 1.5f);
	App->physics->CreateCircle(224, 334, radius, b2_staticBody, 1.5f);

	mesa = App->tex->Load("data/textures/mesa_vacia.png");



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
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15.0f, b2_dynamicBody, 0.0f, true);

	int x;

	//App->win->SetTitle("(%d,%d)");
	

	p2SString title("Mouse: (%d,%d)", 
		App->input->GetMouseX(), App->input->GetMouseY());

	App->win->SetTitle(title.GetString());

	App->render->Blit(mesa, 0, 0);
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

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
