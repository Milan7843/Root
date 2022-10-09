#pragma once

#include "Root/components/Component.h"
#include "Root/engine/RootEngine.h"
#include "simpleprofiler/Profiler.h"

namespace ComponentEngine
{
    /**
     * Start all loaded scripts and components.
     */
    void startScripts();

    /**
     * Update all loaded scripts and components.
     */
    void updateScripts();

    /**
     * Update all rigidbodies' transforms.
     */
    void updateRigidbodiesTransforms();

    /**
     * Render all loaded components.
     */
    void renderComponents();
};

