#include "SimpleText.h"

SimpleText::SimpleText(const char* fontPath)
{
	initialiseCharacterMap(fontPath);
}

SimpleText::~SimpleText()
{
	std::cout << "Simple text instance destroyed" << std::endl;
}

void SimpleText::renderText(unsigned int shader, const char* text, 
    unsigned int width, unsigned int height, 
    float x, float y, glm::vec2 scale, glm::vec3 color)
{
    // Setting up the shader
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.r, color.g, color.b);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    // Iterator through the string
    const char* c = &text[0];
    while (*c != '\0') // Go on until hitting delimitter
    {
        Character ch = characters[*c];

        // Calculating the top-left position of this character
        float xPos{ x + ch.bearing.x * scale.x };
        float yPos{ y - (ch.size.y - ch.bearing.y) * scale.y };

        // Calculating the width and height of the character
        float w = ch.size.x * scale.x;
        float h = ch.size.y * scale.y;

        // Updating the VBO data to contain the exact correct vertex positions
        float vertices[6][4] = {
            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos,     yPos,       0.0f, 1.0f },
            { xPos + w, yPos,       1.0f, 1.0f },

            { xPos,     yPos + h,   0.0f, 0.0f },
            { xPos + w, yPos,       1.0f, 1.0f },
            { xPos + w, yPos + h,   1.0f, 0.0f }
        };

        // Updating content of VBO to hold the updated coordinates
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // Unbinding array buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Setting the texture to the current character's
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Draw call!
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

        // Moving to the next character in the input string
        c++;
    }

    // Unbinding
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SimpleText::initialiseCharacterMap(const char* fontPath)
{
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Failed to initialise FreeType Library" << std::endl;
        return;
    }

    // Loading the font as a face
    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    // Setting size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disabling the restriction for byte-alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // If we got here, the font was successfully loaded
    // Then we go and load the first 128 characters of the ASCII set
    for (unsigned char c{ 0 }; c < 128; c++)
    {
        // Loading character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            // If it fails: display an error message and go to the next character
            std::cout << "ERROR::FREETYPE: Failed to load Glyph at character " << c << std::endl;
            continue;
        }

        // Creating a texture slot, binding it, and filling it with data
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(
            GL_TEXTURE_2D,              // Type
            0,                          // Level
            GL_RED,                     // Channel: only red (it is black/white but red is the first and only channel need)
            face->glyph->bitmap.width,  // Width
            face->glyph->bitmap.rows,   // Height
            0,                          // Border: none
            GL_RED,                     // Format
            GL_UNSIGNED_BYTE,           // Type: unsigned byte = 1 byte
            face->glyph->bitmap.buffer  // Data pointer
        );

        // Setting relevant texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Putting the new data into a Character struct instance,
        // which is then put in the map, corresponding to the current character
        Character character{
            textureID,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), // Width, height
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), // Offset x, y from top left
            static_cast<unsigned int>(face->glyph->advance.x) // Advance (distance) to next glyph
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    // Destroying freetype after using it to extract all data
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Creating the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
