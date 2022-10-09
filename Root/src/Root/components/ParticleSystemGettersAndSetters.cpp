#include "ParticleSystem.h"

void ParticleSystem::setConstantSizeOverLifeTime(glm::vec2 size)
{
    std::vector<GradientPoint<glm::vec2>> sizeGradientPoints{ { 0.0f, size} };
    this->sizeOverLifeTimeGradient = Gradient<glm::vec2>(sizeGradientPoints);
}

void ParticleSystem::setSizeOverLifeTimeGradient(Gradient<glm::vec2> sizeOverLifeTimeGradient)
{
    this->sizeOverLifeTimeGradient = sizeOverLifeTimeGradient;
}

void ParticleSystem::setConstantDragOverLifeTime(float drag)
{
    std::vector<GradientPoint<float>> colorGradientPoints{ { 0.0f, drag} };
    this->dragOverLifeTimeGradient = Gradient<float>(colorGradientPoints);
}

void ParticleSystem::setDragOverLifeTimeGradient(Gradient<float> dragOverLifeTimeGradient)
{
    this->dragOverLifeTimeGradient = dragOverLifeTimeGradient;
}

void ParticleSystem::setConstantColorOverLifeTime(glm::vec3 color)
{
    std::vector<GradientPoint<glm::vec3>> colorGradientPoints{ { 0.0f, color} };
    this->colorOverLifeTimeGradient = Gradient<glm::vec3>(colorGradientPoints);
}

void ParticleSystem::setColorOverLifeTimeGradient(Gradient<glm::vec3> colorOverLifeTimeGradient)
{
    this->colorOverLifeTimeGradient = colorOverLifeTimeGradient;
}

void ParticleSystem::setOnFinish(OnFinish onFinish)
{
    this->onFinish = onFinish;
}

void ParticleSystem::setParticleLimit(unsigned int particleLimit)
{
    this->particleLimit = particleLimit;
}

void ParticleSystem::setSprite(const char* spritePath)
{
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    TextureData textureData(spritePath, &width, &height, &nrComponents, 0);

    if (textureData.hasData())
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData.getData());
        glGenerateMipmap(GL_TEXTURE_2D);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        useTexture = true;
    }
    else
    {
        Logger::logError("Failed to load sprite: " + std::string(spritePath) + ".\nReason: " + textureData.getFailureReason());
    }
}

void ParticleSystem::setEmissionMode(EmissionMode mode)
{
    this->emissionMode = mode;
}

void ParticleSystem::setEmissionDuration(float emissionDuration)
{
    this->emissionDuration = emissionDuration;
}

void ParticleSystem::setEmissionRate(float emissionRate)
{
    this->emissionRate = emissionRate;
}

void ParticleSystem::setEmissionRadius(float emissionRadius)
{
    this->emissionRadius = emissionRadius;
}

void ParticleSystem::setArcEmissionModeMinAngle(float arcModeMinAngle)
{
    this->arcModeMinAngle = arcModeMinAngle;
}

void ParticleSystem::setArcEmissionModeMaxAngle(float arcModeMaxAngle)
{
    this->arcModeMaxAngle = arcModeMaxAngle;
}

void ParticleSystem::setOrthogonalModeAngle(float orthogonalModeAngle)
{
    this->orthogonalModeAngle = orthogonalModeAngle;
}

void ParticleSystem::setGravity(glm::vec2 gravity)
{
    this->gravity = gravity;
}

void ParticleSystem::setWind(glm::vec2 wind)
{
    this->wind = wind;
}

void ParticleSystem::setMinLifeTime(float minLifeTime)
{
    this->minLifeTime = minLifeTime;
}

void ParticleSystem::setMaxLifeTime(float maxLifeTime)
{
    this->maxLifeTime = maxLifeTime;
}

void ParticleSystem::setLifeTime(float lifeTime)
{
    setMinLifeTime(lifeTime);
    setMaxLifeTime(lifeTime);
}

void ParticleSystem::setMinEmissionVelocity(float minEmissionVelocity)
{
    this->minEmissionVelocity = minEmissionVelocity;
}

void ParticleSystem::setMaxEmissionVelocity(float maxEmissionVelocity)
{
    this->maxEmissionVelocity = maxEmissionVelocity;
}

void ParticleSystem::setEmissionVelocity(float emissionVelocity)
{
    setMinEmissionVelocity(emissionVelocity);
    setMaxEmissionVelocity(emissionVelocity);
}

void ParticleSystem::setMinRotationEmissionVelocity(float minRotationEmissionVelocity)
{
    this->minRotationEmissionVelocity = minRotationEmissionVelocity;
}

void ParticleSystem::setMaxRotationEmissionVelocity(float maxRotationEmissionVelocity)
{
    this->maxRotationEmissionVelocity = maxRotationEmissionVelocity;
}

void ParticleSystem::setRotationEmissionVelocity(float rotationEmissionVelocity)
{
    setMinRotationEmissionVelocity(rotationEmissionVelocity);
    setMaxRotationEmissionVelocity(rotationEmissionVelocity);
}

void ParticleSystem::setMinEmissionRotation(float minEmissionRotation)
{
    this->minEmissionRotation = minEmissionRotation;
}

void ParticleSystem::setMaxEmissionRotation(float maxEmissionRotation)
{
    this->maxEmissionRotation = maxEmissionRotation;
}

void ParticleSystem::setEmissionRotation(float emissionRotation)
{
    setMinEmissionRotation(emissionRotation);
    setMaxEmissionRotation(emissionRotation);
}

void ParticleSystem::setRotationFollowVelocity(bool letRotationFollowVelocity)
{
    this->letRotationFollowVelocity = letRotationFollowVelocity;
}
