#include "RootGUI.h"

#include "rootgui/internal/RootGUIInternal.h"

namespace RootGUI
{
    unsigned int getQuadVAO()
    {
        return RootGUIInternal::getQuadVAO();
    }

    void addItemToRenderQueue(RootGUIComponent::Item * item)
    {
        RootGUIInternal::addItemToRenderQueue(item);
    }
};
