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
    glm::vec2 size; // Size of glyph
    glm::vec2 offset; // Offset from baseline to left side of glyph
};

struct Font
{
    unsigned int textureID;  // ID of the character texture atlas
    unsigned int ssbo;
};

struct CharacterUV
{
    glm::vec2 uv;
    glm::vec2 size;
};


namespace TextEngine
{
    void terminate();

    Character* getCharacter(char c);

    void loadFont(const std::string& fontPath, const std::string& tag);

    void setFont(const std::string& tag);

    Font* getFont(const std::string& tag);
};

