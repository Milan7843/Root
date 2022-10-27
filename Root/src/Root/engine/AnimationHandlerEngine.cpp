#include "AnimationHandlerEngine.h"

namespace AnimationHandlerEngine
{
    namespace
    {
        std::vector<TaggedAnimationWeb> animationWebs;

        // getAnimator optimisation: 
        // since many getAnimator calls are likely made contiguously with the same tag,
        // we store the last result and it's tag, so it can be stored first.
        std::string lastGetAnimationWebCallTag{};
        int lastGetAnimationWebCallResultIndex{ -1 }; // -1 signifies not found

        AnimationWeb* getAnimationWeb(const std::string& tag)
        {
            // Check whether this call was made for the second time with the same tag
            if (lastGetAnimationWebCallResultIndex != -1
                && lastGetAnimationWebCallTag == tag)
                // And return the previous call's result if it was
                return &animationWebs[lastGetAnimationWebCallResultIndex].animationWeb;

            for (int i = 0; i < animationWebs.size(); i++)
            {
                if (animationWebs[i].tag == tag)
                {
                    lastGetAnimationWebCallTag = tag;
                    lastGetAnimationWebCallResultIndex = i;
                    return &animationWebs[i].animationWeb;
                }
            }

            return nullptr;
        }
    }

    void addAnimationWeb(AnimationWeb animationWeb, const std::string& tag)
    {
        // Creating a copy of the animation web and putting it into a vector to ensure memory contiguity
        animationWebs.push_back(TaggedAnimationWeb{ animationWeb, tag });

        // Reset the last get call result index
        lastGetAnimationWebCallResultIndex = -1;
    }

    bool removeAnimationWeb(const std::string& tag)
    {
        for (int i = 0; i < animationWebs.size(); i++)
        {
            if (animationWebs[i].tag == tag)
            {
                // Removing the animation web at the index where it is found
                animationWebs.erase(animationWebs.begin() + i);

                // Reset the last get call result index,
                // since the object's index may have changed
                lastGetAnimationWebCallResultIndex = -1;

                // Return true indicating an animation was removed
                return true;
            }
        }

        // Return false indicating no animation was removed
        return false;
    }

    void setAnimationWebParameter(const std::string& animationWebTag, const std::string& parameterTag, bool value)
    {
        AnimationWeb* animationWeb{ getAnimationWeb(animationWebTag) };

        if (animationWeb == nullptr)
        {
            Logger::logError("Tried to access animation web with tag that doesn't exist. (" + animationWebTag + ")");
            return;
        }

        animationWeb->setBool(parameterTag, value);
    }

    void update()
    {
        for (int i = 0; i < animationWebs.size(); i++)
        {
            animationWebs[i].animationWeb.update();
        }

        Profiler::addCheckpoint("Animation updates");
    }
}
