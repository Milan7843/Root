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
		bool debugModeEnabled{ false };

		// Initialise the world without gravity
		b2World world{ b2World(b2Vec2(0.0f, -10.0f)) };

		std::vector<b2Body*> bodiesToDestroy;
		std::queue<BodyEnabledStateChange> bodyEnabledStateChanges;

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

		// Then we update all the enabled states
		while (!bodyEnabledStateChanges.empty())
		{
			BodyEnabledStateChange enabledStateChangeInfo = bodyEnabledStateChanges.front();
			enabledStateChangeInfo.body->SetEnabled(enabledStateChangeInfo.flag);
			bodyEnabledStateChanges.pop();
		}


		bodiesToDestroy.clear();

		world.Step(deltaTime, velocityIterations, positionIterations);

		Profiler::addCheckpoint("Physics step");
	}

	void renderDebugView()
	{
		if (!debugModeEnabled)
			return;

		b2Body* body{ world.GetBodyList() };

		for (unsigned int i{ 0 }; i < world.GetBodyCount(); i++)
		{
			// Retrieving the fixture data
			FixtureData* fixtureData{ 
				reinterpret_cast<FixtureData*>(
					body->GetFixtureList()->GetUserData().pointer
				)
			};

			if (fixtureData == nullptr)
			{
				body = body->GetNext();
				continue;
			}

			// Retrieving the rigidbody
			Rigidbody* rigidbody{ fixtureData->rigidbody };

			rigidbody->renderDebugView();

			body = body->GetNext();
		}
	}

	void enableDebugMode()
	{
		debugModeEnabled = true;
	}

	void disableDebugMode()
	{
		debugModeEnabled = false;
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
		// If the world is not locked, we can destroy the body immediately
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

	bool setBodyEnabled(b2Body* body, bool flag)
	{
		// If the body is already in the correct state: dont change anything
		if (body->IsEnabled() == flag)
		{
			return true;
		}

		// If the world is not locked, we can change the state immediately
		if (!world.IsLocked())
		{
			body->SetEnabled(flag);
			return true;
		}
		else
		{
			// Add it to the queue to be destroyed later
			bodyEnabledStateChanges.emplace(body, flag);

			// Body's state is changed later
			return false;
		}
	}
};