#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
	#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
	#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

// Called before render is available
bool ModulePhysics::Init()
{
	//LOG("Creating Physics environment");
	bool ret = true;

	return ret;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics environment");

	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// Instruct the world to perform a single step of simulation.
	// It is generally best to keep the time step and iterations fixed.
	world->Step(timeStep, velocityIterations, positionIterations);

	// Check for collisions on sensors (collision listener does not work for sensors)
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// get center
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		uint8 green = b->GetType() == b2BodyType::b2_staticBody ? 255 : 0;
		uint8 blue = b->GetType() == b2BodyType::b2_kinematicBody ? 255 : 0;
		
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();

					int32 count = polygonShape->GetVertexCount();

					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));

						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, green, blue);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));

					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, green, blue);
				}
				break;

				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, green, blue);
				}
				break;

				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);

						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, green, blue);

						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);

					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, green, blue);
				}
				break;

				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 255, green, blue);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::Update()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	p2List_item<PhysBody*>* item = bodies.getFirst();

	while(item != NULL)
	{
		delete item->data;
		item = item->next;
	}

	bodies.clear();

	// Delete the whole physics world!
	delete world;

	return true;
}

//
PhysBody* ModulePhysics::AddBody(const SDL_Rect& rect, body_type type, float density, float restitution, bool ccd, bool isSensor)
{
	b2BodyDef body;
	
	switch(type)
	{
		case b_static:
			body.type = b2_staticBody;
			break;

		case b_kinematic:
			body.type = b2_kinematicBody;
			break;

		default:
			body.type = b2_dynamicBody;
			break;
	}

	body.position.Set(PIXEL_TO_METERS(rect.x), PIXEL_TO_METERS(rect.y));
	body.angle = 0.0f;
	body.bullet = ccd;

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box_shape;
	box_shape.SetAsBox(PIXEL_TO_METERS(rect.w/2), PIXEL_TO_METERS(rect.h/2));

	b2FixtureDef box_fixture;
	box_fixture.shape = &box_shape;
	box_fixture.density = density;
	box_fixture.restitution = restitution;
	box_fixture.isSensor = isSensor;

	b->CreateFixture(&box_fixture);

	PhysBody* ret = new PhysBody(b, rect, type);
	bodies.add(ret);

	b->SetUserData(ret);

	return ret;
}
PhysBody* ModulePhysics::AddBody(int x, int y, int diameter, body_type type, float density, float restitution, bool ccd, bool isSensor)
{
	b2BodyDef body;

	switch(type)
	{
		case b_static:
		body.type = b2_staticBody;
		break;

		case b_kinematic:
		body.type = b2_kinematicBody;
		break;

		default:
		body.type = b2_dynamicBody;
		break;
	}

	float radius = PIXEL_TO_METERS(diameter) * 0.5f;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = 0.0f;
	body.bullet = ccd;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef box_fixture;
	box_fixture.shape = &shape;
	box_fixture.density = density;
	box_fixture.restitution = restitution;
	box_fixture.isSensor = isSensor;

	b->CreateFixture(&box_fixture);

	PhysBody* ret = new PhysBody(b, {x,y,diameter,diameter}, type);
	bodies.add(ret);

	b->SetUserData(ret);

	return ret;
}


PhysBody* ModulePhysics::AddBody(const SDL_Rect& rect, int* points, uint count, body_type type, float density, float restitution, bool isSensor)
{
	b2BodyDef body;

	switch(type)
	{
		case b_static:
		body.type = b2_staticBody;
		break;

		case b_kinematic:
		body.type = b2_kinematicBody;
		break;

		default:
		body.type = b2_dynamicBody;
		break;
	}

	body.position.Set(PIXEL_TO_METERS(rect.x), PIXEL_TO_METERS(rect.y));
	body.angle = 0.0f;

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* p = new b2Vec2[count / 2];
	for(uint i = 0; i < count / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(p, count / 2);

	b2FixtureDef box_fixture;
	box_fixture.shape = &shape;
	box_fixture.density = density;
	box_fixture.restitution = restitution;
	box_fixture.isSensor = isSensor;

	b->CreateFixture(&box_fixture);

	PhysBody* ret = new PhysBody(b, {rect.x, rect.y, rect.w, rect.h}, type);
	bodies.add(ret);

	delete[] p;

	b->SetUserData(ret);

	return ret;
}

PhysBody* ModulePhysics::AddEdge(const SDL_Rect& rect, int* points, uint count)
{
	b2BodyDef body;

	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(rect.x), PIXEL_TO_METERS(rect.y));
	body.angle = 0.0f;

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[count / 2];

	for(uint i = 0; i < count / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);// *rect.w;
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);// *rect.h;
	}

	shape.CreateLoop(p, count / 2);

	b2FixtureDef box_fixture;
	box_fixture.shape = &shape;

	b->CreateFixture(&box_fixture);

	PhysBody* ret = new PhysBody(b, rect, b_static);
	bodies.add(ret);

	delete[] p;

	b->SetUserData(ret);

	return ret;
}

void ModulePhysics::DestroyBody(PhysBody* body)
{
	assert(body);
	bodies.del(bodies.findNode(body));
	delete body;
}

void ModulePhysics::CreateRevoluteJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1, int y_pivot_1, int x_pivot_2, int y_pivot_2, int max_angle, int min_angle)
{
	b2RevoluteJointDef def;

	def.bodyA = body_1->body;
	def.bodyB = body_2->body;

	def.localAnchorA.Set(PIXEL_TO_METERS(x_pivot_1), PIXEL_TO_METERS(y_pivot_1));
	def.localAnchorB.Set(PIXEL_TO_METERS(x_pivot_2), PIXEL_TO_METERS(y_pivot_2));

	if(max_angle != INT_MAX && min_angle != INT_MIN)
	{
		def.enableLimit = true;
		def.upperAngle = DEGTORAD * max_angle;
		def.lowerAngle = DEGTORAD * min_angle;
	}

	world->CreateJoint(&def);
}

void ModulePhysics::CreateLineJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1, int y_pivot_1, int x_pivot_2, int y_pivot_2, float frequency, float damping)
{
	b2DistanceJointDef def;

	def.bodyA = body_1->body;
	def.bodyB = body_2->body;

	def.localAnchorA.Set(PIXEL_TO_METERS(x_pivot_1), PIXEL_TO_METERS(y_pivot_1));
	def.localAnchorB.Set(PIXEL_TO_METERS(x_pivot_2), PIXEL_TO_METERS(y_pivot_2));

	def.dampingRatio = damping; // 0 ... 1
	def.frequencyHz = frequency; // < 30.0f

	world->CreateJoint(&def);
}

void ModulePhysics::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	PhysBody* physA = (PhysBody*) contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*) contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}