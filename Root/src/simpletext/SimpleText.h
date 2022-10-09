#pragma once

/*
* Text render based off of https://learnopengl.com/In-Practice/Text-Rendering
* Requires GLFW, GLM, FreeType
*/

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <filesystem>

struct Character
{
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class SimpleText
{
public:
	SimpleText(const char* fontPath);
	~SimpleText();

    // Render the given text
    void renderText(unsigned int shader, const char* text, unsigned int width, unsigned int height, 
        float x, float y, glm::vec2 scale, glm::vec3 color);

private:
    // Initialise the character mapping with textures
    void initialiseCharacterMap(const char* fontPath);

    // The mapping from a character to a specific texture and its data
    std::map<char, Character> characters;

    // The data buffers for a texture quad
    unsigned int VAO{ 0 };
    unsigned int VBO{ 0 };
};

