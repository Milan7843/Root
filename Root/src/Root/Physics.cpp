#include "Physics.h"

#include "Root/engine/PhysicsEngine.h"

namespace Physics
{
    void setCollisionListener(CollisionListener* collisionListener)
    {
        PhysicsEngine::setCollisionListener(collisionListener);
    }
    void setGravity(float x, float y)
    {
        PhysicsEngine::setGravity(x, y);
    }
};

