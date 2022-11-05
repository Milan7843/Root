#include "RootGUI.h"

#include "rootgui/internal/RootGUIInternal.h"
#include "rootgui/internal/TextEngine.h"

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

    void enableDebugMode()
    {
        RootGUIInternal::enableDebugMode();
    }

    void disableDebugMode()
    {
        RootGUIInternal::disableDebugMode();
    }

    void loadFont(const std::string& fontPath,
        const std::string& tag,
        float characterSpacing,
        float spaceWidth,
        unsigned int resolution)
    {
        TextEngine::loadFont(fontPath, tag, characterSpacing, spaceWidth, resolution);
    }
};
