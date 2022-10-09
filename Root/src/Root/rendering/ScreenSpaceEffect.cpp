#include "ScreenSpaceEffect.h"

#include <Root/rendering/Renderer.h>

ScreenSpaceEffect::~ScreenSpaceEffect()
{
	Logger::destructorMessage("Screen space effect");
}

ScreenSpaceEffectPointer ScreenSpaceEffect::create(const std::string& shaderPath)
{
	ScreenSpaceEffect* screenSpaceEffect = new ScreenSpaceEffect(shaderPath);
	std::shared_ptr<ScreenSpaceEffect> pointer{ screenSpaceEffect };
	screenSpaceEffect->self = pointer;
	return pointer;
}

void ScreenSpaceEffect::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool ScreenSpaceEffect::isEnabled()
{
	return this->enabled;
}

ScreenSpaceEffect::ScreenSpaceEffect(const std::string& shaderPath)
	: Shader("include/Root/shaders/default_shader_source/screenSpaceVertex.shader", shaderPath.c_str())
{
}
