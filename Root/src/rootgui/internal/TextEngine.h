#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>

struct Character
{
    glm::vec2 size; // Size of glyph in real size (height [0,1], width proportional)
    glm::vec2 offset; // Offset from baseline to left side of glyph (height [0,1], width proportional)
    glm::vec2 uv; // UV of the glpyh in texture space
    glm::vec2 textureSize; // Size of glyph in texture space
};

struct Font
{
    unsigned int textureID;  // ID of the character texture atlas
    float characterSpacing; // Spacing between each character
    float lineHeight;

    // The mapping from a character to a specific texture and its data
    std::map<char, Character> characters;
};

namespace TextEngine
{
    void terminate();

    Character* getCharacter(Font* font, char c);

    void loadFont(const std::string& fontPath,
        const std::string& tag,
        float characterSpacing,
        float spaceWidth,
        unsigned int resolution);

    void setFont(const std::string& tag);

    Font* getFont(const std::string& tag);
};

