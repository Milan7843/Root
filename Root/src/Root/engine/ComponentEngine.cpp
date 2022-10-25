#include "ComponentEngine.h"

namespace ComponentEngine
{
    void startScripts()
    {
        // Copying the scripts so that the original vector can be modified 
        std::vector<std::shared_ptr<Script>> scripts{ RootEngine::getScripts() };

        // Calling all script start() functions
        for (std::shared_ptr<Script>& script : RootEngine::getScripts())
        {
            if (!script->internal_started())
            {
                script->start();
                script->internal_start();
            }
        }

        // Copying the transforms so that the original vector can be modified 
        std::vector<std::shared_ptr<Transform>> transforms{ RootEngine::getTransforms() };

        // Calling all components' start() functions
        for (std::shared_ptr<Transform> transform : transforms)
        {
            // Copying the components so that the original vector can be modified 
            std::vector<std::shared_ptr<Component>> components{ transform->getComponents() };

            // Calling start() on each component attached to this Transform
            for (std::shared_ptr<Component>& component : components)
            {
                if (!component->internal_started())
                {
                    component->start();
                    component->internal_start();
                }
            }
        }
    }

    void updateScripts()
    {
        startScripts();

        // Calling all script update() functions
        for (std::shared_ptr<Script>& script : RootEngine::getScripts())
        {
            script->update();
        }

        Profiler::addCheckpoint("Script updates");

        // Copying the transforms so that the original vector can be modified 
        std::vector<std::shared_ptr<Transform>> transforms{ RootEngine::getTransforms() };

        // Calling all components' start() functions
        for (std::shared_ptr<Transform> transform : transforms)
        {
            // Copying the components so that the original vector can be modified 
            std::vector<std::shared_ptr<Component>> components{ transform->getComponents() };

            // Calling update() on each component attached to this Transform
            for (std::shared_ptr<Component>& component : components)
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

