#include "Text.h"

#include <rootgui/RootGUI.h>
#include <rootgui/internal/RootGUIInternal.h>

TextPointer RootGUIComponent::Text::create(
    const std::string& text,
    const std::string& fontTag,
    float textSize,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec2 scale)
{
    TextPointer textPtr = new Text(text, fontTag, textSize, position, size, scale);
    std::shared_ptr<Text> pointer(textPtr);
    RootGUI::addItemToRenderQueue(pointer);
    return textPtr;
}

RootGUIComponent::Text::~Text()
{
}

void RootGUIComponent::Text::setColor(glm::vec3 color)
{
    this->color = color;
}

void RootGUIComponent::Text::render(unsigned int guiShader, unsigned int textShader)
{
    RootGUIComponent::Rectangle::render(guiShader, textShader);

    unsigned int shader{ RootGUIInternal::getTextShader() };

    glUseProgram(shader);

    Font* font{ TextEngine::getFont(fontTag) };
    if (font == nullptr)
    {
        std::cout << "ERROR: invalid font tag: " << fontTag << std::endl;
        return;
    }

    glUniform1i(glGetUniformLocation(shader, "text"), 0);
    glUniform3f(glGetUniformLocation(shader, "textColor"), 
        color.x, color.y, color.z);

    // Setting the transform matrix
    glUniformMatrix4fv(glGetUniformLocation(shader, "transform"),
        1, GL_FALSE,
        glm::value_ptr(getTransformMatrix()));

    // Binding the sprite
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->textureID);

    glBindVertexArray(textVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6 * text.length());

    glBindVertexArray(0);
}

RootGUIComponent::Text::Text(
    const std::string& text,
    const std::string& fontTag,
    float textSize,
    glm::vec2 position,
    glm::vec2 size,
    glm::vec2 scale)
    : RootGUIComponent::Rectangle(position, size, scale)
    , text(text)
    , fontTag(fontTag)
    , textSize(textSize)
{
    updateVAO(this->text);
}

float RootGUIComponent::Text::getWordLength(const char* c, Font* font)
{
    float length{ 0.0f };
    if (*c == ' ')
        c++;

    while (*c != ' ' && *c != '\0') // Go on until hitting space or delimitter
    {
        Character* ch = TextEngine::getCharacter(*c);

        if (ch == nullptr)
        {
            continue;

            c++;
        }

        length += (ch->size.x * textSize + font->characterSpacing * 0.01f);
        c++;
    }

    return length;
}

void RootGUIComponent::Text::updateVAO(const std::string& text)
{
    glm::vec2 padding{ 0.02f, 0.02f };
    float wrapWidth{ size.x - padding.x * 2 };

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

    // Start at top-left
    float xOrigin{ -size.x / 2.0f + padding.x };
    float yOrigin{ -font->lineHeight * textSize + size.y / 2.0f - padding.y };

    float xOffset{ 0.0f };
    float yOffset{ 0.0f };

    // Iterator through the string
    const char* c = &text[0];

    float wordLength{ 0.0f };

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
        float xPos{ xOrigin + xOffset + (ch->offset.x) * textSize };
        float yPos{ yOrigin + yOffset + (ch->offset.y) * textSize };
        float xSize{ ch->size.x * textSize };
        float ySize{ ch->size.y * textSize };

        // Reference index for the vertex
        unsigned int vertexStartIndex{ characterIndex * offsetPerCharacter };

        // Setting the positions of the vertices
        vertexData[vertexStartIndex + 0] = xPos;
        vertexData[vertexStartIndex + 1] = yPos;
        vertexData[vertexStartIndex + 2] = ch->uv.x;
        vertexData[vertexStartIndex + 3] = ch->uv.y + ch->textureSize.y;

        vertexData[vertexStartIndex + 4] = xPos;
        vertexData[vertexStartIndex + 5] = yPos - ySize;
        vertexData[vertexStartIndex + 6] = ch->uv.x;
        vertexData[vertexStartIndex + 7] = ch->uv.y;

        vertexData[vertexStartIndex + 8] = xPos + xSize;
        vertexData[vertexStartIndex + 9] = yPos - ySize;
        vertexData[vertexStartIndex + 10] = ch->uv.x + ch->textureSize.x;
        vertexData[vertexStartIndex + 11] = ch->uv.y;

        vertexData[vertexStartIndex + 12] = xPos;
        vertexData[vertexStartIndex + 13] = yPos;
        vertexData[vertexStartIndex + 14] = ch->uv.x;
        vertexData[vertexStartIndex + 15] = ch->uv.y + ch->textureSize.y;

        vertexData[vertexStartIndex + 16] = xPos + xSize;
        vertexData[vertexStartIndex + 17] = yPos - ySize;
        vertexData[vertexStartIndex + 18] = ch->uv.x + ch->textureSize.x;
        vertexData[vertexStartIndex + 19] = ch->uv.y;

        vertexData[vertexStartIndex + 20] = xPos + xSize;
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
        xOffset += xSize + font->characterSpacing * 0.01f;

        // Calculate the word length when encountering a space, or if it hadn't been done yet
        if (*c == ' ' || wordLength == 0.0f)
        {
            wordLength = getWordLength(c, font);

            // Checking for word wrapping
            if (xOffset + wordLength > wrapWidth)
            {
                xOffset = 0.0f;
                yOffset -= font->lineHeight * textSize;
            }
        }

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
