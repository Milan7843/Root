#include "Root.h"

#include "Root/engine/RootEngine.h"

namespace Root
{

    int Start(unsigned int windowWidth, unsigned int windowHeight)
    {
        RootEngine::Start(windowWidth, windowHeight);
        return 0;
    }

    Shader* getSpriteRenderShader()
    {
        return RootEngine::getSpriteRenderShader();
    }


    void addScript(std::shared_ptr<Script> script)
    {
        RootEngine::addScript(script);
    }

    void addTransform(std::shared_ptr<Transform> transform)
    {
        RootEngine::addTransform(transform);
    }

    TransformPointer getTransformByName(std::string& name)
    {
        return RootEngine::getTransformByName(name);
    }

    Camera* getActiveCamera()
    {
        return RootEngine::getActiveCamera();
    }

    unsigned int getScreenWidth()
    {
        return RootEngine::getScreenWidth();
    }

    unsigned int getScreenHeight()
    {
        return RootEngine::getScreenHeight();
    }
};
