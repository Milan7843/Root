#include "Renderer.h"

#include <Root/engine/RendererEngine.h>

namespace Renderer
{
	void initialise(unsigned int width, unsigned int height)
	{
        RendererEngine::initialise(width, height);
	}

    ScreenSpaceEffect* addScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect)
    {
        RendererEngine::addScreenSpaceEffect(screenSpaceEffect);
        return screenSpaceEffect.get();
    }

    unsigned int getSquareVAO()
    {
        return RendererEngine::getSquareVAO();
    }

    unsigned int getScreenRectVAO()
    {
        return RendererEngine::getScreenRectVAO();
    }

    void setMSAAQualityLevel(unsigned int qualityLevel)
    {
        RendererEngine::setMSAAQualityLevel(qualityLevel);
    }

    namespace Bloom
    {
        namespace
        {
            ScreenSpaceEffect* bloom = nullptr;

            void initialiseBloom()
            {
                // Initialising the bloom screen space effect
                bloom = Renderer::addScreenSpaceEffect(ScreenSpaceEffect::create("include/Root/default_shader_source/bloomFragment.shader"));

                bloom->use();

                // Setting default values
                bloom->setEnabled(false);
                bloom->setFloat("threshold", 0.7f);
                bloom->setFloat("intensity", 1.0f);
            }
        }


        void setEnabled(bool enabled)
        {
            // Initialise the bloom if it was not initialised yet
            if (bloom == nullptr)
                initialiseBloom();

            // Then set the parameter
            bloom->use();
            bloom->setEnabled(enabled);
        }

        void setThreshold(float threshold)
        {
            // Initialise the bloom if it was not initialised yet
            if (bloom == nullptr)
                initialiseBloom();

            // Then set the parameter
            bloom->use();
            bloom->setFloat("threshold", threshold);
        }

        void setIntensity(float intensity)
        {
            // Initialise the bloom if it was not initialised yet
            if (bloom == nullptr)
                initialiseBloom();

            // Then set the parameter
            bloom->use();
            bloom->setFloat("intensity", intensity);
        }

        void setRadius(int radius)
        {
            Logger::logWarning("Using non-implemented function Renderer::Bloom::setRadius.");
        }

    }
};
