#include "Renderer.h"

#include <Root/engine/RendererEngine.h>

namespace Renderer
{
	void initialise(unsigned int width, unsigned int height)
	{
        RendererEngine::initialise(width, height);
	}

    void addScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect)
    {
        RendererEngine::addScreenSpaceEffect(screenSpaceEffect);
    }

    unsigned int getSquareVAO()
    {
        return RendererEngine::getSquareVAO();
    }

    unsigned int getScreenRectVAO()
    {
        return RendererEngine::getScreenRectVAO();
    }
};
