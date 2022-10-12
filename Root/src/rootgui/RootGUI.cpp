#include "RootGUI.h"

#include "rootgui/internal/RootGUIInternal.h"

namespace RootGUI
{
    unsigned int getQuadVAO()
    {
        return RootGUIInternal::getQuadVAO();
    }

    void addItemToRenderQueue(std::shared_ptr<RootGUIComponent::Item> item)
    {
        RootGUIInternal::addItemToRenderQueue(item);
    }
    unsigned int getWindowWidth()
    {
        return 0;
    }
    unsigned int getWindowHeight()
    {
        return 0;
    }
};
