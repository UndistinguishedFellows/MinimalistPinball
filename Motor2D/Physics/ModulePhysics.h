#pragma once
#include "../j1Module.h"
#include "../p2Defs.h"
#include "Box2D\Box2D\Box2D.h"



#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL 0.02f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other j1Modules to track position and rotation of physics bodies


enum body_type
{
	b_dynamic,
	b_static,
	b_kinematic
};

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
	PhysBody(b2Body* body, const SDL_Rect& rect, body_type type);
	void GetPosition(int& x, int &y) const;
	b2Vec2 GetPosition() const;
	//void SetPosition(int x, int y, float angle = 0.0f);
	void PhysBody::SetPosition(int x, int y)
	{
		body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
	}
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void PhysBody::SetLinearSpeed(int x, int y)
	{
		body->SetLinearVelocity(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)));
	}

	void PhysBody::SetAngularSpeed(float speed)
	{
		body->SetAngularVelocity(speed * DEGTORAD);
	}
	void Turn(int degrees)
	{
		body->ApplyAngularImpulse(DEGTORAD * degrees, true);
	}
	void Push(float x, float y)
	{
		body->ApplyForceToCenter(b2Vec2(x, y), true);
	}

public:
	int width, height;
	b2Body* body;
	j1Module* listener;


	SDL_Rect rect;
	body_type type;
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

	PhysBody* CreateRectangle(int x, int y, int width, int height, bool dynamic, float angle, float restitution = 0);
	PhysBody* CreateRectangle(int x, int y, int width, int height, bool dynamic);
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type = b2_dynamicBody, float restitution = 0.0f, bool bullet = false);

	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float angle);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type, float restitution = 0);
	PhysBody* CreateChain(int x, int y, p2List<int>* points, int size, b2BodyType type, float restitution = 0);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType type, float restitution = 0);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType type, float restitution = 0, float angle = 0);
	void DeleteBody(PhysBody* body);
	void CreatePrismaticJoint(PhysBody* body_1, PhysBody* body_2, bool coll_conect, int low_trans, int up_trans, bool limits, int max_motor_force, float motor_speed, bool motor);
	/*b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* body_1, PhysBody* body_2, bool coll_conect, int anchor_A_X, int anchor_A_Y, int anchor_B_X, int anchor_B_Y, bool limits, float lower_angle, float upper_angle, float torque, float motor_speed, bool motor);*/
	b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(const PhysBody* a, const PhysBody* b, const b2Vec2& Center_a, const b2Vec2 Center_b, const bool limit, const int lowAngle, const int upAngle, const int motorSpeed, const int maxTorque);


	void CreateRevoluteJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1 = 0, int y_pivot_1 = 0, int x_pivot_2 = 0, int y_pivot_2 = 0, int max_angle = INT_MAX, int min_angle = INT_MIN);
	PhysBody* AddBody(int x, int y, int diameter, body_type type = b_dynamic, float density = 1.0f, float restitution = 0.0f, bool ccd = false, bool isSensor = false);
	PhysBody* AddBody(const SDL_Rect& rect, int* points, uint count, body_type type = b_dynamic, float density = 1.0f, float restitution = 0.0f, bool isSensor = false);

	void ModulePhysics::DeleteJoint(b2Joint* joint);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	b2World* GetWorld()
	{
		return world;
	}
	b2MouseJoint* mouse_joint;
private:

	bool debug;
	b2World* world;

	b2Body* ground;
	b2Vec2 gravity;
	int pixels_per_meter;
	float meter_per_pixel;

	b2Body* clicked;
	b2Vec2 mouse_position;

	p2List<PhysBody*> bodies;
};