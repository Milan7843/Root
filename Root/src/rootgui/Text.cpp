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

    glUniform1i(glGetUniformLocation(RootGUIInternal::getTextShader(), "text"), 0);
    glUniform3f(glGetUniformLocation(RootGUIInternal::getTextShader(), "textColor"), 
        color.x, color.y, color.z);

    glUniformMatrix4fv(
        glGetUniformLocation(RootGUIInternal::getTextShader(), "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(RootGUIInternal::getProjectionMatrix()));

    // Binding the sprite
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->textureID);

    glBindVertexArray(textVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6 * text.length());

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
    Font* font{ TextEngine::getFont(fontTag) };
    if (font == nullptr)
    {
        std::cout << "ERROR: invalid font tag: " << fontTag << std::endl;
        return;
    }

    // Buffer with space for:
    // 6 vertices per character, each with 2 floats for pos, 2 floats for uv.
    float* vertexData = (float*)malloc(text.length() * 6 * 2 * 2 * sizeof(float));

    unsigned int offsetPerCharacter{ 6 * 2 * 2 };

    unsigned int characterIndex{ 0 };

    float xOffset{ 0.0f };

    // Iterator through the string
    const char* c = &text[0];

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
        vertexData[vertexStartIndex + 0] = xPos;
        vertexData[vertexStartIndex + 1] = yPos;
        vertexData[vertexStartIndex + 2] = ch->uv.x;
        vertexData[vertexStartIndex + 3] = ch->uv.y + ch->textureSize.y;

        vertexData[vertexStartIndex + 4] = xPos;
        vertexData[vertexStartIndex + 5] = yPos - ch->size.y;
        vertexData[vertexStartIndex + 6] = ch->uv.x;
        vertexData[vertexStartIndex + 7] = ch->uv.y;

        vertexData[vertexStartIndex + 8] = xPos + ch->size.x;
        vertexData[vertexStartIndex + 9] = yPos - ch->size.y;
        vertexData[vertexStartIndex + 10] = ch->uv.x + ch->textureSize.x;
        vertexData[vertexStartIndex + 11] = ch->uv.y;

        vertexData[vertexStartIndex + 12] = xPos;
        vertexData[vertexStartIndex + 13] = yPos;
        vertexData[vertexStartIndex + 14] = ch->uv.x;
        vertexData[vertexStartIndex + 15] = ch->uv.y + ch->textureSize.y;

        vertexData[vertexStartIndex + 16] = xPos + ch->size.x;
        vertexData[vertexStartIndex + 17] = yPos - ch->size.y;
        vertexData[vertexStartIndex + 18] = ch->uv.x + ch->textureSize.x;
        vertexData[vertexStartIndex + 19] = ch->uv.y;

        vertexData[vertexStartIndex + 20] = xPos + ch->size.x;
        vertexData[vertexStartIndex + 21] = yPos;
        vertexData[vertexStartIndex + 22] = ch->uv.x + ch->textureSize.x;
        vertexData[vertexStartIndex + 23] = ch->uv.y + ch->textureSize.y;

        /*
        std::cout << "======== Character " << *c << " bounds ========\n";
        std::cout << "Start: " << xPos << ", " << yPos << "\n";
        std::cout << "End: " << xPos + ch->size.x << ", " << yPos + ch->size.y << "\n";
        std::cout << "UV Start: " << ch->uv.x << ", " << ch->uv.y << "\n";
        std::cout << "UV End: " << ch->uv.x + ch->textureSize.x << ", " << ch->uv.y + ch->textureSize.y;
        std::cout << std::endl;
        */
        // Moving to the next character in the input string
        characterIndex++;
        xOffset += ch->size.x + font->characterSpacing;
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
    glBufferData(GL_ARRAY_BUFFER, text.length() * 6 * 2 * 2 * sizeof(float), vertexData, GL_STATIC_DRAW);

    // Letting OpenGL know how to interpret the data:
    // 2 floats for position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 2 floats for uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
