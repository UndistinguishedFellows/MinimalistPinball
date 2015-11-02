#pragma once
#include "../j1Module.h"
#include "../p2Defs.h"
#include "Box2D\Box2D\Box2D.h"



#define PIXELS_PER_METER 50
#define METER_PER_PIXEL 0.02

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other j1Modules to track position and rotation of physics bodies

class Chain
{
public:
	p2List<int>* points;
	float restitution;
};


class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	j1Module* listener;
};

// j1Module --------------------------------------
class ModulePhysics : public j1Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(j1App* app, bool start_enabled = true);
	~ModulePhysics();
	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type);
	PhysBody* CreateChain(int x, int y, p2List<int>* points, int size, b2BodyType type, float restitution);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Vec2 gravity;
	int pixels_per_meter;
	float meter_per_pixel;

};