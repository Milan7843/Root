#include "ParticleSystem.h"

#include "Root/Transform.h"
#include "Root/Random.h"
#include "Root/engine/RootEngine.h"

#include <Root/Math.h>

ParticleSystemPointer ParticleSystem::create(TransformPointer transform)
{
	ParticleSystem* particleSystem = new ParticleSystem();
	std::shared_ptr<ParticleSystem> pointer{ particleSystem };
	transform->addComponent(pointer);
	return particleSystem;
}
ParticleSystem::ParticleSystem()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Making sure everything gets put on this specific VAO
    glBindVertexArray(VAO);

    // Binding the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Letting OpenGL know how to interpret the data:
    // vec2 for position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vec3 for color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // vec2 for size
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // float for rotation
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    Logger::destructorMessage("Particle system");
}

void ParticleSystem::start()
{
}

void ParticleSystem::update()
{
    // Updating particle data
    for (unsigned int i{ 0 }; i < particleDrawData.size(); i++)
    {
        float lifePoint = particleUpdateData[i].aliveTime / particleUpdateData[i].lifeTime;

        // Gravity
        particleUpdateData[i].velocity += gravity * Time::getDeltaTime();
        // Wind
        particleUpdateData[i].velocity += wind * Time::getDeltaTime();
        // Drag
        particleUpdateData[i].velocity
            *= (1.0f - dragOverLifeTimeGradient.sample(lifePoint)
            * Time::getDeltaTime());
        
        // Applying velocity
        particleDrawData[i].position += particleUpdateData[i].velocity * Time::getDeltaTime();

        // Applying angular velocity
        if (letRotationFollowVelocity)
        {
            particleDrawData[i].rotation
                = glm::degrees(
                    glm::atan(particleUpdateData[i].velocity.y, particleUpdateData[i].velocity.x));
        }
        else
        {
            particleDrawData[i].rotation
                += particleUpdateData[i].rotationVelocity
                * Time::getDeltaTime();
        }

        // Setting particle color
        particleDrawData[i].color = colorOverLifeTimeGradient.sample(lifePoint);
        particleDrawData[i].size = sizeOverLifeTimeGradient.sample(lifePoint);

        // Increasing alive time
        particleUpdateData[i].aliveTime += Time::getDeltaTime();

        // Erasing a particle if it is too old
        if (particleUpdateData[i].aliveTime > particleUpdateData[i].lifeTime)
        {
            particleUpdateData.erase(particleUpdateData.begin() + i);
            particleDrawData.erase(particleDrawData.begin() + i);
        }
    }

    // Emitting new particles
    if (emitting)
    {
        while (particlesEmittedThisRun / glm::max(currentEmissionTime, 0.0001f) < emissionRate)
            emitParticle();
    }

    currentEmissionTime += Time::getDeltaTime();

    // Check for finished:
    if (currentEmissionTime > emissionDuration)
    {
        switch (onFinish)
        {
            case STOP:
                emitting = false;
                break;

            case REPEAT:
                // Just keep going
                play();
                break;

            case DESTROY_SELF:
                // Stop emitting right away
                emitting = false;
                // And wait for each particle to go away,
                // when they are all gone, destroy self
                if (this->particleDrawData.size() == 0)
                    this->transform->removeComponent(this);
                break;

            case DESTROY_TRANSFORM:
                // Stop emitting right away
                emitting = false;
                // And wait for each particle to go away,
                // when they are all gone, destroy the parent transform
                if (this->particleDrawData.size() == 0)
                    this->transform->destroy();
                break;
        }
    }
}

void ParticleSystem::render(float renderDepth)
{
    RootEngine::getParticleRenderShader()->use();
    RootEngine::getParticleRenderShader()
        ->setMat4("model", transform->getModelMatrix());

    RootEngine::getParticleRenderShader()
        ->setMat4("projection", Root::getActiveCamera()->getProjectionMatrix());
    RootEngine::getParticleRenderShader()->setInt("sprite", 0);
    RootEngine::getParticleRenderShader()
        ->setFloat("renderDepth", renderDepth / 10000.0f);
    RootEngine::getParticleRenderShader()->setBool("useTexture", useTexture);

    writeDataToVAO();

    // Binding the sprite
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, particleDrawData.size());

    glBindVertexArray(0);
}

void ParticleSystem::play()
{
    emitting = true;
    float currentEmissionTime = 0.0f;
    float particlesEmittedThisRun = 0.0f;
}

void ParticleSystem::stop()
{
    emitting = false;
}

void ParticleSystem::writeDataToVAO()
{
    // Making sure everything gets put on this specific VAO
    glBindVertexArray(VAO);

    // Binding the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Putting the draw data into the buffer
    glBufferData(GL_ARRAY_BUFFER,
        particleDrawData.size() * sizeof(ParticleDrawData),
        particleDrawData.data(),
        GL_STATIC_DRAW);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::emitParticle()
{
    glm::vec2 direction{ getRandomDirection() };
    glm::vec2 position{ transform->getPosition() + getRandomPosition(direction) };
    glm::vec3 color{ colorOverLifeTimeGradient.sample(0.0f) };
    glm::vec2 size{ sizeOverLifeTimeGradient.sample(0.0f) };

    float lifeTime{ Random::between(minLifeTime, maxLifeTime) };

    float velocity{ Random::between(minEmissionVelocity, maxEmissionVelocity) };
    float rotationVelocity{ 
        Random::between(minRotationEmissionVelocity, maxRotationEmissionVelocity) };
    float rotation{ Random::between(minEmissionRotation, maxEmissionRotation) };

    particleDrawData.push_back({
            position,
            color,
            size,
            rotation
        });
    particleUpdateData.push_back({
            direction * velocity,
            0.0f,
            lifeTime,
            rotationVelocity
        });

    particlesEmittedThisRun++;
}

glm::vec2 ParticleSystem::getRandomDirection()
{
    float emissionAngle{ 0.0f };

    switch (emissionMode) {

    case ARC_EMISSION:
        emissionAngle = Random::between(arcModeMinAngle, arcModeMaxAngle);
        break;

    case ORTHOGONAL_EMISSION:
        emissionAngle = orthogonalModeAngle;
        break;
    }

    return glm::vec2(glm::cos(glm::radians(emissionAngle)), glm::sin(glm::radians(emissionAngle)));
}

glm::vec2 ParticleSystem::getRandomPosition(glm::vec2 direction)
{
    switch (emissionMode)
    {
        case ARC_EMISSION:
            return direction * 2.0f * emissionRadius * Random::between(0, 1);

        case ORTHOGONAL_EMISSION:
            // Creating a counter clockwise perpendicular vector to the direction
            glm::vec2 orthogonalVector(-direction.y, direction.x);
            return orthogonalVector * 2.0f * emissionRadius * Random::between(-1, 1);
    }
}
