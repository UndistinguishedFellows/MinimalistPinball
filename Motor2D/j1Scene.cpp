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
	chains = new p2List<p2List<int>*>();
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
		
		LOG("-----------------------------------Chain Childs: %d", App->fs->getNumberOfNodes(iterator));
		
		p2List<int>* chain = new p2List<int>();
		chains->add(chain);
		for (pugi::xml_node pointIterator = iterator.child("point");
			pointIterator != NULL;
			pointIterator = pointIterator.next_sibling())
		{
			chains->end->data->add(pointIterator.attribute("x").as_int());
			chains->end->data->add(pointIterator.attribute("y").as_int());
		}
	}

	
	

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	for (p2List_item<p2List<int>*>* iterator = chains->start; 
		iterator != NULL; 
		iterator = iterator->next)
	{
		App->physics->CreateChain(0, 0, iterator->data, iterator->data->count(), b2_staticBody);
	}
	

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
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 3);
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
