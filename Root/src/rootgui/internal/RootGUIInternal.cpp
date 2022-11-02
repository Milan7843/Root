#include "RootGUIInternal.h"

namespace RootGUIInternal
{
    namespace
    {
        std::vector<std::shared_ptr<RootGUIComponent::Item>> renderQueue;

        glm::mat4 projectionMatrix;

        unsigned int guiShader{ 0 };
        unsigned int textShader{ 0 };

        unsigned int windowWidthUsing{ 0 };
        unsigned int windowHeightUsing{ 0 };

        unsigned int quadVAO{ 0 };
        unsigned int quadVBO{ 0 };
        unsigned int quadEBO{ 0 };

        void createShaderPrograms()
        {
            /* GUI SHADER */

            const char* guiVertexShaderCode{
                "#version 460 core\n"
                "layout(location = 0) in vec2 vertexPos;\n"
                "out vec2 TexCoords;\n"
                "\n"
                "uniform vec2 position;\n"
                "uniform vec2 size;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4((vertexPos * size + position) * 2 - vec2(1.0), 0.0, 1.0);\n"
                "    TexCoords = vertexPos;\n"
                "}\0"
            };
            const char* guiFragmentShaderCode{
                "#version 460 core\n"
                "in vec2 TexCoords;\n"
                "out vec4 FragColor;\n"
                "\n"
                "uniform sampler2D textureSampler;\n"
                "uniform vec3 baseColor;\n"
                "uniform bool useTexture;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    if (useTexture) {\n"
                "       // Texture to be implemented\n"
                "       FragColor = texture(textureSampler, TexCoords);\n"
                "    }\n"
                "    else {\n"
                "       FragColor = vec4(baseColor, 1.0);\n"
                "    }\n"
                "}\0"
            };

            unsigned int guiVertex, guiFragment;
            guiVertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(guiVertex, 1, &guiVertexShaderCode, NULL);
            glCompileShader(guiVertex);

            int success;
            char infoLog[512];
            glGetShaderiv(guiVertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(guiVertex, 512, NULL, infoLog);
                std::cout << "Internal vertex shader compilation failed: " << infoLog << std::endl;
            }


            guiFragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(guiFragment, 1, &guiFragmentShaderCode, NULL);
            glCompileShader(guiFragment);

            // Fragment shader compilation error check
            glGetShaderiv(guiFragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(guiFragment, 512, NULL, infoLog);
                std::cout << "Internal fragment shader compilation failed: " << infoLog << std::endl;
            }

            /* Creating the shader program */
            guiShader = glCreateProgram();
            glAttachShader(guiShader, guiVertex);
            glAttachShader(guiShader, guiFragment);

            glLinkProgram(guiShader);

            glGetProgramiv(guiShader, GL_LINK_STATUS, &success);
            if (!success)
            {
                // Getting the error log
                glGetProgramInfoLog(guiShader, 512, NULL, infoLog);

                // Printing the error log
                std::cout << "Internal shader linking failed: " << infoLog << std::endl;
                return;
            }

            // Deleting the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(guiVertex);
            glDeleteShader(guiFragment);



            /* TEXT SHADER */

            const char* textVertexShaderCode{
                "#version 460 core\n"
                "layout(location = 0) in vec2 pos;\n"
                "layout(location = 1) in vec2 uv;\n"
                "\n"
                "out vec2 TexCoords;\n"
                "\n"
                "uniform mat4 projection;\n"
                "\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection * vec4(pos, 0.0, 1.0);\n"
                "   TexCoords = uv;\n"
                "}\0"
            };
            const char* textFragmentShaderCode{
                "#version 460 core\n"
                "in vec2 TexCoords;\n"
                "out vec4 color;\n"
                "\n"
                "uniform sampler2D text;\n"
                "uniform vec3 textColor;\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
                "    color = vec4(textColor, 1.0) * sampled;\n"
                "}\0"
            };

            unsigned int textVertex, textFragment;
            textVertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(textVertex, 1, &textVertexShaderCode, NULL);
            glCompileShader(textVertex);

            // Vertex shader compilation error check
            glGetShaderiv(textVertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(textVertex, 512, NULL, infoLog);
                std::cout << "Internal text vertex shader compilation failed: " << infoLog << std::endl;
            }

            textFragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(textFragment, 1, &textFragmentShaderCode, NULL);
            glCompileShader(textFragment);

            // Fragment shader compilation error check
            glGetShaderiv(textFragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(textFragment, 512, NULL, infoLog);
                std::cout << "Internal text fragment shader compilation failed: " << infoLog << std::endl;
            }

            /* Creating the shader program */
            textShader = glCreateProgram();
            glAttachShader(textShader, textVertex);
            glAttachShader(textShader, textFragment);

            glLinkProgram(textShader);

            // Deleting the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(textVertex);
            glDeleteShader(textFragment);
        }
    }

    void initialise(unsigned int windowWidth, unsigned int windowHeight)
    {
        setWindowSize(windowWidth, windowHeight);

        createShaderPrograms();

        /* INITIALISING THE QUAD VAO */

        // Creating our vertex array object
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        // Putting the vertices into the array buffer
        float vertices[] = {
            // Position
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
        };
        unsigned int indices[] = {
            1, 0, 2,
            1, 2, 3
        };

        // Making a buffer with the ID in VBO
        glGenBuffers(1, &quadVBO);
        // Binding our new buffer to the GL_ARRAY_BUFFER target
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        // Binding our custom data into the buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Generating a buffer for the EBO
        glGenBuffers(1, &quadEBO);
        // Binding the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        // Inserting data into the buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Telling OpenGL how to interpret the data
        // Position data
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void terminate()
    {
        // Deleting every pointer in the render queue
        /*
        for (std::shared_ptr<RootGUIComponent::Item>& p : renderQueue)
        {
            delete p;
        }
        */

        renderQueue.clear();

        TextEngine::terminate();

        std::cout << "RootGUI terminated." << std::endl;
    }

    void render()
    {
        // Draw GUI on top of everything
        glDisable(GL_DEPTH_TEST);

        for (std::shared_ptr<RootGUIComponent::Item>& item : renderQueue)
        {
            item->render(guiShader, textShader);
        }
    }

    unsigned int getTextShader()
    {
        return textShader;
    }

    unsigned int getQuadVAO()
    {
        return quadVAO;
    }

    glm::mat4& getProjectionMatrix()
    {
        return projectionMatrix;
    }

    void addItemToRenderQueue(std::shared_ptr<RootGUIComponent::Item> item)
    {
        renderQueue.push_back(item);
    }

    void setWindowSize(unsigned int windowWidth, unsigned int windowHeight)
    {
        windowWidthUsing = windowWidth;
        windowHeightUsing = windowHeight;

        // Calculating the new projection matrix
        projectionMatrix = glm::ortho(0.0f, (float)windowWidthUsing, 0.0f, (float)windowHeightUsing, -1.0f, 1.0f);
    }

    unsigned int getWindowWidth()
    {
        return windowWidthUsing;
    }

    unsigned int getWindowHeight()
    {
        return windowHeightUsing;
    }
};

