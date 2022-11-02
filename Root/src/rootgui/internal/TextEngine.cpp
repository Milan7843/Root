#include "TextEngine.h"

namespace TextEngine
{
    namespace
    {
        std::map<std::string, Font> fonts;

        // The mapping from a character to a specific texture and its data
        std::map<char, Character> characters;
    }

    void terminate()
    {
        // Deleting all buffers
        for (std::pair<std::string, Font> font : fonts)
        {
            glDeleteTextures(1, &font.second.textureID);
        }
    }

    Character* getCharacter(char c)
    {
        std::map<char, Character>::iterator iterator = characters.find(c);

        // Found the character
        if (iterator != characters.end())
            return &iterator->second;

        // Did not find the character
        return nullptr;
    }

    void loadFont(const std::string& fontPath, const std::string& tag)
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
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font " << fontPath << std::endl;
            return;
        }

        // Setting size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disabling the restriction for byte-alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned int textureWidth{ 0 };
        unsigned int textureHeight{ 0 };
        unsigned int characterSpacing{ 1 }; // Number of pixel in between each character

        // First calculating the total width and height required for the texture atlas
        for (unsigned char c{ 0 }; c < 128; c++)
        {
            // Loading character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                // If it fails: display an error message and go to the next character
                std::cout << "ERROR::FREETYPE: Failed to load Glyph at character " << c << std::endl;
                continue;
            }

            FT_Bitmap* bitmap = &face->glyph->bitmap;

            textureWidth += bitmap->width;
            if (bitmap->rows > textureHeight)
            {
                textureHeight = bitmap->rows;
            }
        }

        textureWidth += 127 * characterSpacing;

        char* pixels = (char*)calloc(textureWidth * textureHeight, sizeof(char));

        unsigned int widthOffset{ 0 };

        // If we got here, the font was successfully loaded
        // Then we go and load the first 128 characters of the ASCII set
        for (unsigned char c{ 0 }; c < 128; c++)
        {
            // Loading character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                // If it fails: go to the next character
                continue;
            }

            //std::cout << "Loading character: " << c << std::endl;

            FT_Bitmap* bitmap = &face->glyph->bitmap;

            for (unsigned int row = 0; row < bitmap->rows; ++row)
            {
                for (unsigned int col = 0; col < bitmap->width; ++col)
                {
                    unsigned int x = widthOffset + col;
                    unsigned int y = textureHeight - row - 1;
                    pixels[y * textureWidth + x] = bitmap->buffer[row * bitmap->pitch + col];
                }
            }

            Character character{
                glm::vec2(bitmap->width, bitmap->rows) / (float)textureHeight * 0.1f, // Width, height
                glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top) / (float)textureHeight * 0.1f, // Offset from top left
                glm::vec2(widthOffset / (float)textureWidth, 1.0f - (bitmap->rows / (float)textureHeight)), // UV
                //glm::vec2(widthOffset / (float)textureWidth, 0.0f), // UV
                glm::vec2(bitmap->width / (float)textureWidth, bitmap->rows / (float)textureHeight) // Texture space size
                //glm::vec2(bitmap->width / (float)textureWidth, 1.0f) // Texture space size
            };

            /*
            std::cout << "======== Character " << c << " ========\n";
            std::cout << "Size: " << character.size.x << ", " << character.size.y << "\n";
            std::cout << "Offset: " << character.offset.x << ", " << character.offset.y << "\n";
            std::cout << "UV: " << character.uv.x << ", " << character.uv.y << "\n";
            std::cout << "Texture size: " << character.textureSize.x << ", " << character.textureSize.y;
            std::cout << std::endl;
            */
            characters.emplace(c, character);

            widthOffset += bitmap->width + characterSpacing;
        }

        // Creating a texture slot, binding it, and filling it with data
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(
            GL_TEXTURE_2D,              // Type
            0,                          // Level
            GL_RED,                     // Channel: only red (it is black/white but red is the first and only channel need)
            textureWidth,               // Width
            textureHeight,              // Height
            0,                          // Border: none
            GL_RED,                     // Format
            GL_UNSIGNED_BYTE,           // Type: unsigned byte = 1 byte
            pixels                      // Data pointer
        );

        // Setting relevant texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "======== FONT TEXTURE DATA ========\n";
        std::cout << "Width: " << textureWidth << "px\n";
        std::cout << "Height: " << textureHeight << "px\n";
        std::cout << std::endl;

        fonts.emplace(tag, Font{ textureID } );

        // Destroying freetype after using it to extract all data
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void setFont(const std::string& tag)
    {
    }

    Font* getFont(const std::string& tag)
    {
        std::map<std::string, Font>::iterator iterator = fonts.find(tag);

        // Found the font
        if (iterator != fonts.end())
            return &iterator->second;

        // Did not find the font
        return nullptr;
    }
};