#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum body_type
{
	b_dynamic,
	b_static,
	b_kinematic
};

class PhysBody
{
public:

	PhysBody(b2Body* body, const SDL_Rect& rect, body_type type);
	~PhysBody();

	double GetAngle() const;
	void GetPosition(int& x, int& y) const;
	void SetLinearSpeed(int x, int y);
	void SetAngularSpeed(float speed);
	void Push(float x, float y);
	void Turn(int degrees);
	void SetPosition(int x, int y);

public:

	b2Body* body;
	Module* listener;

private:

	SDL_Rect rect;
	body_type type;
};

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* AddBody(const SDL_Rect& rect, body_type type = b_dynamic, float density = 1.0f, float restitution = 0.0f, bool ccd = false, bool isSensor = false);
	PhysBody* AddBody(int x, int y, int diameter, body_type type = b_dynamic, float density = 1.0f, float restitution = 0.0f, bool ccd = false, bool isSensor = false);
	PhysBody* AddBody(const SDL_Rect& rect, int* points, uint count, body_type type = b_dynamic, float density = 1.0f, float restitution = 0.0f, bool isSensor = false);
	PhysBody* AddEdge(const SDL_Rect& rect, int* points, uint count);

	void CreateRevoluteJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1 = 0, int y_pivot_1 = 0, int x_pivot_2 = 0, int y_pivot_2 = 0, int max_angle = INT_MAX, int min_angle = INT_MIN);
	void CreateLineJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1 = 0, int y_pivot_1 = 0, int x_pivot_2 = 0, int y_pivot_2 = 0, float frequency = 15.0f, float damping = 0.5f);
	void DestroyBody(PhysBody* body);

	//void BeginContact(b2Contact* contact);
	//void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	
private:

	b2World* world;
	p2List<PhysBody*> bodies;
	bool debug;
};