#include "RendererEngine.h"

#include "Root/engine/RootEngine.h"

namespace RendererEngine
{
    namespace
    {
        unsigned int mainFrameBuffer{ 0 };

        unsigned int screenDestinationTexture{ 0 };
        unsigned int screenSourceTexture{ 0 };
        unsigned int depthRenderBuffer{ 0 };

        unsigned int squareVAO{ 0 };
        unsigned int screenRectVAO{ 0 };

        unsigned int savedScreenWidth{ 0 };
        unsigned int savedScreenHeight{ 0 };

        std::vector<ScreenSpaceEffectPointer> screenSpaceEffects;

        Shader* screenTextureShader;

        void createSquareVAO()
        {
            // Creating the VAO for a square to draw 

            unsigned int squareVBO, squareEBO;
            float s = 0.5f;
            float vertices[] = {
                // Positions
                s,  s, 1.0f, 1.0f, // top right
                s, -s, 1.0f, 0.0f, // bottom right
                -s, -s, 0.0f, 0.0f, // bottom left
                -s,  s, 0.0f, 1.0f, // top left 
            };

            unsigned int indices[] = {
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };

            // Generating the required objects
            glGenVertexArrays(1, &squareVAO);
            glGenBuffers(1, &squareVBO);
            glGenBuffers(1, &squareEBO);

            // Making sure everything gets put on this specific VAO
            glBindVertexArray(squareVAO);

            // Binding the buffers
            glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);

            // Putting the indices and vertices into the buffers
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Letting OpenGL know how to interpret the data:
            // 2 floats for position
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Another 2 floats for UV's
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Unbinding
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void createScreenRectVAO()
        {
            // Creating the VAO for a screen rectangle 

            unsigned int screenRectVBO, screenRectEBO;
            float s = 1.0f;
            float vertices[] = {
                // Positions
                s,  s, 1.0f, 1.0f, // top right
                s, -s, 1.0f, 0.0f, // bottom right
                -s, -s, 0.0f, 0.0f, // bottom left
                -s,  s, 0.0f, 1.0f, // top left 
            };

            unsigned int indices[] = {
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };

            // Generating the required objects
            glGenVertexArrays(1, &screenRectVAO);
            glGenBuffers(1, &screenRectVBO);
            glGenBuffers(1, &screenRectEBO);

            // Making sure everything gets put on this specific VAO
            glBindVertexArray(screenRectVAO);

            // Binding the buffers
            glBindBuffer(GL_ARRAY_BUFFER, screenRectVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenRectEBO);

            // Putting the indices and vertices into the buffers
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Letting OpenGL know how to interpret the data:
            // 2 floats for position
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // Another 2 floats for UV's
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Unbinding
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void runScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect)
        {
            // Swapping source and destination, as all draw data was in destination
            unsigned int temp = screenDestinationTexture;
            screenDestinationTexture = screenSourceTexture;
            screenSourceTexture = temp;

            // Now the drawn frame data is in source
            // and destination is the destination for the drawing of the effect

            glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);

            glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                screenDestinationTexture,
                0);


            screenSpaceEffect->use();
            screenSpaceEffect->setInt("baseTexture", 0);
            screenSpaceEffect->setVector2("windowSize",
                RootEngine::getScreenWidth(),
                RootEngine::getScreenHeight());

            glBindVertexArray(getScreenRectVAO());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screenSourceTexture);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            // After all this, the new draw data is in destination
        }
    };

    void initialise(unsigned int width, unsigned int height)
    {
        glGenFramebuffers(1, &mainFrameBuffer);
        setTextureSize(width, height);

        screenTextureShader = new Shader(
            "include/Root/default_shader_source/screenSpaceVertex.shader",
            "include/Root/default_shader_source/screenSpaceFragment.shader");

        createSquareVAO();
        createScreenRectVAO();
    }

    void newFrame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);
    }

    void displayFrame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Clearing the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screenTextureShader->use();
        screenTextureShader->setInt("baseTexture", 0);
        screenTextureShader->setVector2("windowSize", savedScreenWidth, savedScreenHeight);

        glBindVertexArray(getScreenRectVAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenDestinationTexture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void addScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect)
    {
        screenSpaceEffects.push_back(screenSpaceEffect);
    }

    void runScreenSpaceEffects()
    {
        for (ScreenSpaceEffectPointer& screenSpaceEffect : screenSpaceEffects)
        {
            if (screenSpaceEffect->isEnabled())
                runScreenSpaceEffect(screenSpaceEffect);
        }

        Profiler::addCheckpoint("Screen space effects");
    }

    void setTextureSize(unsigned int width, unsigned int height)
    {
        // If the screen size didn't change, don't continue this function
        if (savedScreenWidth == width && savedScreenHeight == height)
            return;

        savedScreenWidth = width;
        savedScreenHeight = height;

        // Generating the source texture, not linked to the framebuffer
        glGenTextures(1, &screenSourceTexture);
        glBindTexture(GL_TEXTURE_2D, screenSourceTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


        // Then generating the destination texture, which the framebuffer renders to
        glBindFramebuffer(GL_FRAMEBUFFER, mainFrameBuffer);

        glGenTextures(1, &screenDestinationTexture);
        glBindTexture(GL_TEXTURE_2D, screenDestinationTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenDestinationTexture, 0);
        
        // Creating the depth buffer as a 
        glGenRenderbuffers(1, &depthRenderBuffer);

        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

        /*
        unsigned int depthRenderBuffer;
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void terminate()
    {
        glDeleteVertexArrays(1, &squareVAO);
        glDeleteVertexArrays(1, &screenRectVAO);
        glDeleteFramebuffers(1, &mainFrameBuffer);
    }

    void setMSAAQualityLevel(unsigned int qualityLevel)
    {
        glEnable(GL_MULTISAMPLE);

        if (qualityLevel == 1)
            glDisable(GL_MULTISAMPLE);

        glfwWindowHint(GLFW_SAMPLES, glm::pow(2, qualityLevel));
    }

    unsigned int getSquareVAO()
    {
        return squareVAO;
    }

    unsigned int getScreenRectVAO()
    {
        return screenRectVAO;
    }
};
