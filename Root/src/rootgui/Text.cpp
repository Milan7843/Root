#include "Text.h"

#include <rootgui/RootGUI.h>
#include <rootgui/internal/RootGUIInternal.h>

TextPointer RootGUIComponent::Text::create(
    const std::string& text,
    const std::string& fontTag, 
    glm::vec2 position,
    glm::vec2 size,
    glm::vec2 scale)
{
    TextPointer textPtr = new Text(text, fontTag, position, size, scale);
    std::shared_ptr<Text> pointer(textPtr);
    RootGUI::addItemToRenderQueue(pointer);
    return textPtr;
}

RootGUIComponent::Text::~Text()
{
}

void RootGUIComponent::Text::render(unsigned int guiShader, unsigned int textShader)
{
    glUseProgram(RootGUIInternal::getTextShader());

    Font* font{ TextEngine::getFont(fontTag) };
    if (font == nullptr)
    {
        std::cout << "ERROR: invalid font tag: " << fontTag << std::endl;
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, font->ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, font->ssbo);

    glBindVertexArray(textVAO);

    glDrawArrays(GL_TRIANGLES, 0, 12 * text.length());

    glBindVertexArray(0);
}

RootGUIComponent::Text::Text(const std::string& text,
    const std::string& fontTag,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec2 scale)
    : text(text)
    , fontTag(fontTag)
{
    updateVAO(this->text);
}

void RootGUIComponent::Text::updateVAO(const std::string& text)
{
    // Buffer with space for 12 floats per character
    float* vertices = (float*)malloc(text.length() * 12 * sizeof(float));

    // Iterator through the string
    const char* c = &text[0];

    unsigned int offsetPerCharacter{ 12 }; // 6 vertices per character *2 = 12 floats

    unsigned int characterIndex{ 0 };

    float xOffset{ 0.0f };

    while (*c != '\0') // Go on until hitting delimitter
    {
        Character* ch = TextEngine::getCharacter(*c);

        if (ch == nullptr)
        {
            std::cout << "Failed to load character " << *c << std::endl;

            continue;

            characterIndex++;
            c++;
        }

        // Calculating the top-left position of this character
        float xPos{ xOffset + ch->offset.x };
        float yPos{ ch->offset.y };

        // Reference index for the vertex
        unsigned int vertexStartIndex{ characterIndex * offsetPerCharacter };

        // Setting the positions of the vertices
        vertices[vertexStartIndex + 0]  = xPos;
        vertices[vertexStartIndex + 1]  = yPos + ch->size.y;

        vertices[vertexStartIndex + 2]  = xPos;
        vertices[vertexStartIndex + 3]  = yPos;

        vertices[vertexStartIndex + 4]  = xPos + ch->size.x;
        vertices[vertexStartIndex + 5]  = yPos;

        vertices[vertexStartIndex + 6]  = xPos;
        vertices[vertexStartIndex + 7]  = yPos + ch->size.y;

        vertices[vertexStartIndex + 8]  = xPos + ch->size.x;
        vertices[vertexStartIndex + 9]  = yPos;

        vertices[vertexStartIndex + 10] = xPos + ch->size.x;
        vertices[vertexStartIndex + 11] = yPos + ch->size.y;

        // Moving to the next character in the input string
        characterIndex++;
        xOffset += ch->size.x;
        c++;
    }

    // Putting the vertex data into a vertex buffer

    unsigned int VBO;

    // Generating the required objects
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &VBO);

    // Making sure everything gets put on this specific VAO
    glBindVertexArray(textVAO);

    // Binding the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Putting the vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Letting OpenGL know how to interpret the data:
    // 2 floats for position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
