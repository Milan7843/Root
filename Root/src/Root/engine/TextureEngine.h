#pragma once

#include <Root/Logger.h>

#include <stbi_utility/TextureData.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

struct Texture
{
	std::string path;
	bool pixelPerfect;
	unsigned int textureID;
};

namespace TextureEngine
{
	/**
	 * Load a texture into a buffer.
	 *
	 * \param spritePath: the path to the texture.
	 * \param pixelPerfect: whether the texture should be pixelated or not.
	 * \returns the index of the buffer the texture is in.
	 */
	unsigned int loadTexture(const std::string& spritePath, bool pixelPerfect);
};

