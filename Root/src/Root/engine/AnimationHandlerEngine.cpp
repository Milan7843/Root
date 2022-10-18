#include "AnimationHandlerEngine.h"

namespace AnimationHandlerEngine
{
    namespace
    {
        std::vector<TaggedAnimator> animators;

        // getAnimator optimisation: 
        // since many getAnimator calls are likely made contiguously with the same tag,
        // we store the last result and it's tag, so it can be stored first.
        std::string lastGetAnimatorCallTag{};
        int lastGetAnimatorCallResultIndex{ -1 }; // -1 signifies not found

        Animator* getAnimator(const std::string& tag)
        {
            // Check whether this call was made for the second time with the same tag
            if (lastGetAnimatorCallResultIndex != -1
                && lastGetAnimatorCallTag == tag)
                // And return the previous call's result if it was
                return &animators[lastGetAnimatorCallResultIndex].animator;

            for (int i = 0; i < animators.size(); i++)
            {
                if (animators[i].tag == tag)
                {
                    lastGetAnimatorCallTag = tag;
                    lastGetAnimatorCallResultIndex = i;
                    return &animators[i].animator;
                }
            }

            return nullptr;
        }
    }

    void addAnimator(Animator animator, const std::string& tag)
    {
        // Creating a copy of the animator and putting it into a vector to ensure memory contiguity
        animators.push_back(TaggedAnimator{ animator, tag });
    }

    void setAnimatorParameter(const std::string& animatorTag, const std::string& parameterTag, bool value)
    {
        Animator* animator{ getAnimator(animatorTag) };

        if (animator == nullptr)
        {
            Logger::logError("Tried to access animator with tag that doesn't exist.");
        }

        animator->setBool(parameterTag, value);
    }

    void update()
    {
        for (int i = 0; i < animators.size(); i++)
        {
            animators[i].animator.update();
        }
    }
}
