#pragma once

#include "Root/shaders/Shader.h"

#include <memory>
#include <string>

#define ScreenSpaceEffectPointer std::shared_ptr<ScreenSpaceEffect>

/**
 * Screen space effect shader template:

#version 460 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D baseTexture;
uniform vec2 windowSize;

void main()
{
	FragColor = texture(baseTexture, textureCoords);
}

 */

/**
 * Class that encompasses a shader that will be run for every pixel on the screen,
 * allowing for full-screen post-processing effects.
 * Go to the class definition to see a template for a screen space effect shader.
 */
class ScreenSpaceEffect : public Shader
{
public:

	~ScreenSpaceEffect();

	/**
	 * Create a new screen space effect.
	 *
	 * \param shaderPath: the path to the shader that will be run as the effect.
	 * \returns a ScreenSpaceEffectPointer pointing to the newly made effect.
	 */
	static ScreenSpaceEffectPointer create(const std::string& shaderPath);

	/**
	 * Set whether this screen space effect is currently enabled.
	 * 
	 * \param enabled: whether the effect should be enabled.
	 */
	void setEnabled(bool enabled);

	/**
	 * Get whether this screen space effect is currently enabled.
	 */
	bool isEnabled();

private:

	// Private constructor: use create()
	ScreenSpaceEffect(const std::string& shaderPath);

	ScreenSpaceEffectPointer self;

	const char* vertexShaderPath{"include/Root/shaders/default_shader_source/screenSpaceVertex.shader"};

	// Start enabled
	bool enabled{ true };
};

