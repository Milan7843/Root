#include "TextureEngine.h"

namespace TextureEngine
{
	namespace
	{
		std::vector<Texture> loadedTextures;

		Texture* getLoaded(const std::string& texturePath, bool pixelPerfect)
		{
			// Comparing the given identifying values to each loaded texture's values
			for (Texture& texture : loadedTextures)
			{
				if (texture.pixelPerfect == pixelPerfect
					&& texture.path == texturePath)
				{
					return &texture;
				}
			}

			return nullptr;
		}
	}

	unsigned int loadTexture(const std::string& spritePath, bool pixelPerfect)
	{
		// Check whether the texture has already been loaded:
		Texture* loaded = getLoaded(spritePath, pixelPerfect);
		if (loaded != nullptr)
		{
			Logger::log("Loaded texture used.");
			return loaded->textureID;
		}

		// Otherwise generate a new one
		Texture newTexture{ spritePath, pixelPerfect, 0 };

		glGenTextures(1, &newTexture.textureID);

		int width, height, nrComponents;
		TextureData textureData(spritePath.c_str(), &width, &height, &nrComponents, 0);

		if (textureData.hasData())
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, newTexture.textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData.getData());
			glGenerateMipmap(GL_TEXTURE_2D);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelPerfect ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelPerfect ? GL_NEAREST : GL_LINEAR);
		}
		else
		{
			Logger::logError("Failed to load texture: " + std::string(spritePath) + ".\nReason: " + textureData.getFailureReason());
		}

		// Adding the new texture to the list of loaded textures
		loadedTextures.push_back(newTexture);
		Logger::log("Generating new texture...");

		return newTexture.textureID;
	}
};
