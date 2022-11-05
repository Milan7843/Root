#pragma once

#include "Item.h"
#include "Rectangle.h"
#include <rootgui/internal/TextEngine.h>

#include <vector>

#define TextPointer RootGUIComponent::Text*

namespace RootGUIComponent
{
	class Text : public RootGUIComponent::Rectangle
	{
	public:

		/**
		 * Create a new text object.
		 *
		 * \param text: the text to display.
		 * \param fontTag: the tag of the font to use.
		 * \param textSize: the size of the text.
		 * \param position: the position of the rectangle (default = [0, 0]).
		 * \param size: the size of the rectangle (default = [1, 1]).
		 * \param scale: the position of the rectangle (default = [1, 1]).
		 * \returns a pointer to the new rectangle.
		 */
		static TextPointer create(
			const std::string& text,
			const std::string& fontTag,
			float textSize,
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 size = glm::vec2(1.0f),
			glm::vec2 scale = glm::vec2(1.0f));

		~Text();

		/**
		 * Set the text color.
		 * 
		 * \param color: the text color for alpha for transparency.
		 */
		void setTextColor(glm::vec4 color);

		/**
		 * Set the text color.
		 *
		 * \param color: the text color.
		 */
		void setTextColor(glm::vec3 color);

		// Render this GUI text
		void render(unsigned int guiShader, unsigned int textShader);

		/**
		 * Set whether to center this text vertically.
		 *
		 * \param centerHorizontally: true to center this text vertically.
		 */
		void setCenterVertically(bool centerVertically);

		/**
		 * Set whether to center this text horizontally.
		 * 
		 * \param centerHorizontally: true to center this text horizontally.
		 */
		void setCenterHorizontally(bool centerHorizontally);

	protected:

		Text(
			const std::string& text,
			const std::string& fontTag,
			float textSize,
			glm::vec2 position,
			glm::vec2 size,
			glm::vec2 scale);

		void updateVAO(const std::string& text);

		float getWordLength(const char* c, Font* font);

		void centerLine(
			float xOrigin,
			float xOffset,
			const char* c,
			unsigned int vertexStartIndex,
			const char* firstCharacterOfCurrentLine,
			float* vertexData,
			unsigned int indexOffsetPerCharacter,
			const char* begin);

		void renderDebugView();

		std::string text;
		std::string fontTag;

		float textSize;

		bool centerVertically{ false };
		bool centerHorizontally{ false };

		glm::vec4 textColor{ glm::vec4(1.0f) };

		unsigned int textVAO;
	};
};

