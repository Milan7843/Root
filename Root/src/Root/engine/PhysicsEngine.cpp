#include "PhysicsEngine.h"


namespace PhysicsEngine
{
	namespace
	{
		float gravity{};

		int32 velocityIterations{ 6 };
		int32 positionIterations{ 2 };
		float timeStep{ 1.0f / 60.0f };
		b2Body* body;

		// Initialise the world without gravity
		b2World world{ b2World(b2Vec2(0.0f, -10.0f)) };

		std::vector<b2Body*> bodiesToDestroy;

		CollisionCallbackHandler collisionCallbackHandler;
	}

	void initialise()
	{
		world.SetContactListener(&collisionCallbackHandler);
	}

	void simulate()
	{
		while (RootEngine::isPhysicsSimulationActive())
		{
			auto t_start = std::chrono::high_resolution_clock::now();


			const std::clock_t beginTime = clock();

			// Time step here
			step(timeStep);

			float waitTime = timeStep - (float(clock() - beginTime) / CLOCKS_PER_SEC);

			int milliseconds = (waitTime * 1000.0f);

			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

			auto t_end = std::chrono::high_resolution_clock::now();
			double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

			//std::cout << elapsed_time_ms << "ms" << std::endl;
		}
	}

	void step(float deltaTime)
	{
		// First we delete all bodies waiting to be destroyed
		for (b2Body*& body : bodiesToDestroy)
		{
			world.DestroyBody(body);
		}

		bodiesToDestroy.clear();

		world.Step(deltaTime, velocityIterations, positionIterations);
	}

	void setGravity(float x, float y)
	{
		world.SetGravity(b2Vec2(x, y));
	}

	void setCollisionListener(CollisionListener* contactListener)
	{
		collisionCallbackHandler.setCollisionListener(contactListener);
	}

	b2Body* addBody(b2BodyDef* definition)
	{
		body = world.CreateBody(definition);
		return body;
	}

	bool destroyBody(b2Body* bodyToDestroy)
	{
		if (!world.IsLocked())
		{
			world.DestroyBody(bodyToDestroy);
			return true;
		}
		else
		{
			// Add it to the queue to be destroyed later
			bodiesToDestroy.push_back(bodyToDestroy);
			return false;
		}
	}
};