#include "Text.h"

#include <rootgui/RootGUI.h>
#include <rootgui/internal/RootGUIInternal.h>

TextPointer RootGUIComponent::Text::create(
    const std::string& text,
    const std::string& fontTag,
    float textSize,
    glm::vec2 position,
    glm::vec2 size,
    float rotation)
{
    TextPointer textPtr = new Text(text, fontTag, textSize, position, size, rotation);
    std::shared_ptr<Text> pointer(textPtr);
    RootGUI::addItemToRenderQueue(pointer);
    return textPtr;
}

RootGUIComponent::Text::~Text()
{
}

void RootGUIComponent::Text::setTextColor(glm::vec4 color)
{
    this->textColor = color;
}

void RootGUIComponent::Text::setTextColor(glm::vec3 color)
{
    this->textColor = glm::vec4(color, 1.0f);
}

void RootGUIComponent::Text::setTextColorOnHover(glm::vec4 color)
{
    textColorDifferenceOnInteract.addValue(InteractionStatus::Hovered, color);
}

void RootGUIComponent::Text::setTextColorOnHover(glm::vec3 color)
{
    textColorDifferenceOnInteract.addValue(InteractionStatus::Hovered, glm::vec4(color, 1.0f));
}

void RootGUIComponent::Text::setTextColorOnPress(glm::vec4 color)
{
    textColorDifferenceOnInteract.addValue(InteractionStatus::Pressed, color);
}

void RootGUIComponent::Text::setTextColorOnPress(glm::vec3 color)
{
    textColorDifferenceOnInteract.addValue(InteractionStatus::Pressed, glm::vec4(color, 1.0f));
}

void RootGUIComponent::Text::setTransitionDuration(float transitionDuration)
{
    RootGUIComponent::Rectangle::setTransitionDuration(transitionDuration);

    colorDifferenceOnInteract.setTransitionDuration(transitionDuration);
}

void RootGUIComponent::Text::setTextPadding(glm::vec2 padding)
{
    this->padding = padding;
    updateVAO(text);
}

void RootGUIComponent::Text::render(unsigned int guiShader, unsigned int textShader)
{
    RootGUIComponent::Rectangle::render(guiShader, textShader);

    unsigned int shader{ RootGUIInternal::getTextShader() };

    glm::vec2 scaleDifferenceByInteract{ scaleDifferenceOnInteract.sample() };
    glm::vec4 textColorDifferenceByInteract{ textColorDifferenceOnInteract.sample() };

    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "additionalScale"),
        scaleDifferenceByInteract.x, scaleDifferenceByInteract.y);

    Font* font{ TextEngine::getFont(fontTag) };
    if (font == nullptr)
    {
        std::cout << "ERROR: invalid font tag: " << fontTag << std::endl;
        return;
    }

    glm::vec4 textColorUsing{ textColor * textColorDifferenceByInteract };

    glUniform1i(glGetUniformLocation(shader, "text"), 0);
    glUniform4f(glGetUniformLocation(shader, "textColor"), 
        textColorUsing.r, textColorUsing.g, textColorUsing.b, textColorUsing.a);

    // Setting the transform matrix
    glUniformMatrix4fv(glGetUniformLocation(shader, "transform"),
        1, GL_FALSE,
        glm::value_ptr(getModelMatrixWithoutScale()));

    // Binding the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->textureID);

    // Binding the vertex data
    glBindVertexArray(textVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6 * text.length());

    glBindVertexArray(0);

    renderDebugView();
}

void RootGUIComponent::Text::renderDebugView()
{
    if (!RootGUIInternal::isInDebugMode())
        return;

    // Switching to wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int shader{ RootGUIInternal::getTextDebugShader() };

    glm::vec2 scaleDifferenceByInteract{ scaleDifferenceOnInteract.sample() };

    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "additionalScale"),
        scaleDifferenceByInteract.x, scaleDifferenceByInteract.y);

    // Setting the transform matrix
    glUniformMatrix4fv(glGetUniformLocation(shader, "transform"),
        1, GL_FALSE,
        glm::value_ptr(getModelMatrixWithoutScale()));

    // Binding the vertex data
    glBindVertexArray(textVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6 * text.length());

    glBindVertexArray(0);

    // Switching back to regular render mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RootGUIComponent::Text::setInteractionStatus(InteractionStatus status)
{
    RootGUIComponent::Rectangle::setInteractionStatus(status);

    textColorDifferenceOnInteract.setActiveValue(status);
}

void RootGUIComponent::Text::setCenterVertically(bool centerVertically)
{
    this->centerVertically = centerVertically;
    updateVAO(text);
}

void RootGUIComponent::Text::setCenterHorizontally(bool centerHorizontally)
{
    this->centerHorizontally = centerHorizontally;
    updateVAO(text);
}

RootGUIComponent::Text::Text(
    const std::string& text,
    const std::string& fontTag,
    float textSize,
    glm::vec2 position,
    glm::vec2 size,
    float rotation)
    : RootGUIComponent::Rectangle(position, size, rotation)
    , text(text)
    , fontTag(fontTag)
    , textSize(textSize)
{
    updateVAO(this->text);
    textColorDifferenceOnInteract.addValue(InteractionStatus::None, glm::vec4(1.0f));
}

float RootGUIComponent::Text::getWordLength(const char* c, Font* font)
{
    float length{ 0.0f };
    if (*c == ' ')
        c++;

    while (*c != ' ' && *c != '\0') // Go on until hitting space or delimitter
    {
        Character* ch = TextEngine::getCharacter(font, *c);

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

void RootGUIComponent::Text::centerLine(
    float xOrigin,
    float xOffset,
    const char* c,
    unsigned int vertexStartIndex,
    const char* firstCharacterOfCurrentLine,
    float* vertexData,
    unsigned int indexOffsetPerCharacter,
    const char* begin)
{
    // Finding the mean x-value of the last line,
    // and putting it in the middle
    // xOrigin is the left side of the first character,
    // xOffset the right side of the last one
    float mean{ xOrigin + (xOffset) / 2.0f };
    float targetMean{ 0.0f };
    float diff{ mean - targetMean };

    // Iterating back and adjusting the x-values
    const char* ci{ c + 1 };

    // The index we are currently at
    unsigned int iteratingIndex{ vertexStartIndex };


    if (ci - 1 != firstCharacterOfCurrentLine)
        do
        {
            ci--;
            // Every 4th index holds x position data, which needs to be modified
            for (unsigned int i{ 0 }; i < 6; i++)
            {
                vertexData[iteratingIndex + i * 4] -= diff;
            }

            iteratingIndex -= indexOffsetPerCharacter;

        } while (ci != firstCharacterOfCurrentLine && ci != &text[0]);
}

void RootGUIComponent::Text::updateVAO(const std::string& text)
{
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

    // The number of indices to offset to the next character:
    // 6 vertices, each with 2 times 2 floats (pos, uv)
    unsigned int indexOffsetPerCharacter{ 6 * 2 * 2 };

    unsigned int characterIndex{ 0 };

    // Start at top-left
    float xOrigin{ -size.x / 2.0f + padding.x };
    float yOrigin{ -font->lineHeight * textSize + size.y / 2.0f - padding.y };

    float xOffset{ 0.0f };
    float yOffset{ 0.0f };

    // Iterator through the string
    const char* c = &text[0];

    float wordLength{ 0.0f };

    // Keeps track of the first character of the current line,
    // So that we can iterate back to it
    const char* firstCharacterOfCurrentLine{ c };

    while (*c != '\0') // Go on until hitting delimitter
    {
        Character* ch = TextEngine::getCharacter(font, *c);

        if (ch == nullptr)
        {
            std::cout << "Failed to load character " << *c << std::endl;

            characterIndex++;
            c++;

            continue;
        }

        // Calculating the top-left position of this character
        float xPos{ xOrigin + xOffset + (ch->offset.x) * textSize };
        float yPos{ yOrigin + yOffset + (ch->offset.y) * textSize };
        float xSize{ ch->size.x * textSize };
        float ySize{ ch->size.y * textSize };

        // Reference index for the vertex
        unsigned int vertexStartIndex{ characterIndex * indexOffsetPerCharacter };

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
            // Finding the length of the word we are in
            wordLength = getWordLength(c, font);

            // Checking for word wrapping
            if (xOffset + wordLength > wrapWidth || *(c + 1) == '\0')
            {
                // Centering the last line horizontally if needed
                if (centerHorizontally)
                {
                    centerLine(xOrigin,
                        xOffset - (font->characterSpacing * 0.01f),
                        c,
                        vertexStartIndex,
                        firstCharacterOfCurrentLine,
                        vertexData,
                        indexOffsetPerCharacter,
                        &text[0]);
                }

                // Moving back left
                xOffset = 0.0f;
                // And a line down
                yOffset -= font->lineHeight * textSize;

                // The next character will be the first of a new line
                firstCharacterOfCurrentLine = c + 1;
            }
        }

        // Also center the last line
        if (*(c + 1) == '\0')
        {
            if (centerHorizontally)
            {
                centerLine(xOrigin,
                    xOffset - (font->characterSpacing * 0.01f),
                    c,
                    vertexStartIndex,
                    firstCharacterOfCurrentLine,
                    vertexData,
                    indexOffsetPerCharacter,
                    &text[0]);
            }
        }

        c++;
    }

    if (centerVertically)
    {
        // Calculating how far to move each character
        float mean{ yOrigin + (yOffset + font->lineHeight * textSize) / 2.0f };
        float targetMean{ 0.0f };
        float diff{ mean - targetMean };

        // Updating each character's y position
        for (unsigned int i{ 0 }; i < text.length(); i++)
        {
            // Every (4k + 1)th index holds y position data, which needs to be modified
            for (unsigned int j{ 0 }; j < 6; j++)
            {
                vertexData[(i * 24) + (j * 4) + 1] -= diff;
            }
        }
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
