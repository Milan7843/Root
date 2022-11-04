#include "RootEngine.h"

#include "Root/rendering/Camera.h"

namespace RootEngine
{

    // 'Private' members of the namespace
    namespace
    {
        unsigned int WINDOW_SIZE_X, WINDOW_SIZE_Y;

        std::vector<std::shared_ptr<Transform>> transforms;
        std::vector<std::shared_ptr<Script>> scripts;

        Shader* spriteRenderShader;
        Shader* particleRenderShader;
        Shader* debugRenderShader;

        Camera* activeCamera{ nullptr };
        GLFWwindow* window{ nullptr };

        bool physicsSimulationActive{ true };

        void initialiseGLFW()
        {
            // Initialising GLFW
            glfwInit();

            // Setting OpenGL version 4.6
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

            // OpenGL profile: core
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        void initialiseImGui(GLFWwindow* window)
        {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style to be dark
            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 130");
        }
    }

    int Start(unsigned int windowWidth, unsigned int windowHeight)
    {
        WINDOW_SIZE_X = windowWidth;
        WINDOW_SIZE_Y = windowHeight;

        initialiseGLFW();

        // Making a GLFW window
        window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "OpenGL", NULL, NULL);
        if (window == NULL)
        {
            Logger::logError("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        // GLAD manages function pointers for OpenGL, so we cannot run without it
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::logError("Failed to initialize GLAD");
            return -1;
        }

        initialiseImGui(window);

        spriteRenderShader = new Shader(
            "include/Root/default_shader_source/spriteVertex.shader",
            "include/Root/default_shader_source/spriteFragment.shader");

        debugRenderShader = new Shader(
            "include/Root/default_shader_source/debugVertex.shader",
            "include/Root/default_shader_source/debugFragment.shader");

        particleRenderShader = new Shader(
            "include/Root/default_shader_source/particleVertex.shader",
            "include/Root/default_shader_source/particleFragment.shader",
            "include/Root/default_shader_source/particleGeometry.shader");

        // Shader textRenderShader("src/shaders/textVertex.shader", "src/shaders/textFragment.shader");

        // Enabling depth testing for rasterized view: 
        // makes sure objects get drawn on top of each other in the correct order
        glEnable(GL_DEPTH_TEST);

        AudioEngine::initialise();

        RendererEngine::initialise(WINDOW_SIZE_X, WINDOW_SIZE_Y);

        PhysicsEngine::initialise();

        InputEngine::initialise();

        unsigned int frame = 0;

        RootGUIInternal::initialise(WINDOW_SIZE_X, WINDOW_SIZE_Y);

        //std::thread physicsSimulation(PhysicsEngine::simulate);

        while (!glfwWindowShouldClose(window))
        {
            Profiler::addCheckpoint("Start of frame");

            // Updating the variables in Time
            Time::update(glfwGetTime());

            // Getting viewport size
            glfwGetWindowSize(window, (int*)&WINDOW_SIZE_X, (int*)&WINDOW_SIZE_Y);
            RendererEngine::setTextureSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
            RootGUIInternal::setWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);

            // Setting viewport size
            glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

            // Updating the input engine
            InputEngine::update();

            RootGUIInternal::update(InputEngine::getMouseNormalizedScreenPosition(), InputEngine::getMouseButton(MOUSE_LEFT));

            // Calling all component and script start() and update() functions
            ComponentEngine::updateScripts();

            AudioEngine::update();

            AnimationHandlerEngine::update();

            PhysicsEngine::step(Time::getDeltaTime());

            ComponentEngine::updateRigidbodiesTransforms();

            // Input
            //processInput(window);

            // Rendering

            // Starting a new frame for rendering
            RendererEngine::newFrame();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Enabling depth testing for rasterized view: 
            // makes sure objects get drawn on top of each other in the correct order
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            // Calling all component render() functions
            ComponentEngine::renderComponents();

            RendererEngine::runScreenSpaceEffects();

            RendererEngine::displayFrame();

            glDisable(GL_DEPTH_TEST);

            PhysicsEngine::renderDebugView();

            RootGUIInternal::render();

            InputEngine::newFrame();

            Profiler::addCheckpoint("End of frame");

            // Rendering and resetting profiler
            Profiler::createProfilerWindow();
            Profiler::emptyCheckpointList();

            frame++;

            // Output
            glfwSwapBuffers(window);
            // Check for input
            glfwPollEvents();
        }

        physicsSimulationActive = false;

        //physicsSimulation.join();

        terminateRoot();

        RendererEngine::terminate();

        AudioEngine::terminate();

        Logger::stop();

        glfwTerminate();

        return 0;
    }

    Shader* getSpriteRenderShader()
    {
        return spriteRenderShader;
    }

    Shader* getDebugRenderShader()
    {
        return debugRenderShader;
    }

    Shader* getParticleRenderShader()
    {
        return particleRenderShader;
    }


    void addScript(std::shared_ptr<Script> script)
    {
        scripts.push_back(script);
    }

    std::vector<std::shared_ptr<Script>>& getScripts()
    {
        return scripts;
    }

    void addTransform(std::shared_ptr<Transform> transform)
    {
        transforms.push_back(transform);
    }

    void removeTransform(Transform* transformToRemove)
    {
        for (unsigned int i{ 0 }; i < transforms.size(); i++)
        {
            if (transforms[i].get() == transformToRemove)
            {
                transforms.erase(transforms.begin() + i);
                return;
            }
        }
    }

    std::vector<std::shared_ptr<Transform>>& getTransforms()
    {
        return transforms;
    }

    TransformPointer getTransformByName(std::string& name)
    {
        // Comparing each transform's name with the input name
        for (std::shared_ptr<Transform>& transform : transforms)
        {
            // If it matches, return its pointer
            if (transform->getName() == name)
                return transform.get();
        }
        // If no transform was found with teh given name, return a nullptr
        return nullptr;
    }

    void setActiveCamera(Camera* camera)
    {
        activeCamera = camera;
    }

    Camera* getActiveCamera()
    {
        return activeCamera;
    }

    GLFWwindow* getActiveWindow()
    {
        return window;
    }

    unsigned int getScreenWidth()
    {
        return WINDOW_SIZE_X;
    }

    unsigned int getScreenHeight()
    {
        return WINDOW_SIZE_Y;
    }

    void terminateRoot()
    {
        // Removing all links between transforms
        for (std::shared_ptr<Transform> transform : transforms)
        {
            transform->setParent(NULL);
            transform->removeAllChildren();
        }
        transforms.clear();
    }

    bool isPhysicsSimulationActive()
    {
        return physicsSimulationActive;
    }
};
