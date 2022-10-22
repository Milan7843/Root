#include "ComponentEngine.h"

namespace ComponentEngine
{
    void startScripts()
    {
        // Calling all script start() functions
        for (std::shared_ptr<Script>& script : RootEngine::getScripts())
        {
            script->start();
        }

        // Calling all components' start() functions
        for (std::shared_ptr<Transform>& transform : RootEngine::getTransforms())
        {
            // Calling start() on each component attached to this Transform
            for (std::shared_ptr<Component>& component : transform->getComponents())
            {
                component->start();
            }
        }
    }

    void updateScripts()
    {
        // Calling all script update() functions
        for (std::shared_ptr<Script>& script : RootEngine::getScripts())
        {
            script->update();
        }

        Profiler::addCheckpoint("Script updates");

        // Calling all components' update() functions
        for (std::shared_ptr<Transform>& transform : RootEngine::getTransforms())
        {
            // Calling update() on each component attached to this Transform
            for (std::shared_ptr<Component>& component : transform->getComponents())
            {
                component->update();
            }
        }

        Profiler::addCheckpoint("Component script updates");
    }

    void updateRigidbodiesTransforms()
    {
        // Calling all components' update() functions
        for (std::shared_ptr<Transform>& transform : RootEngine::getTransforms())
        {
            RigidbodyPointer rigidbody{ transform->getAttachedRigidbody() };

            if (rigidbody != nullptr)
                rigidbody->updateTransform();
        }

        Profiler::addCheckpoint("Rigidbody updates");
    }

    void renderComponents()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Don't render if there is no active camera
        if (RootEngine::getActiveCamera() == nullptr)
            return;

        // Calling all components' render() functions
        for (std::shared_ptr<Transform>& transform : RootEngine::getTransforms())
        {
            // Only render the root transforms
            if (transform->getParent() == NULL)
                transform->render();
        }

        Profiler::addCheckpoint("Component rendering");
    }
};

