#include "Math.h"

namespace Math
{
	template <>
	glm::ivec2 cerp<glm::ivec2>(glm::ivec2 v1, glm::ivec2 v2, float t)
	{
		return glm::ivec2(cerp(v1.x, v2.x, t), cerp(v1.y, v2.y, t));
	}

	float move(float value, float target, float speed)
	{
		float diff = (target - value);
		// Check for too little distance
		if (glm::abs(diff) < 0.00001f) {
			return value;
		}

		float move = glm::sign(diff) * speed * Time::getDeltaTime();

		// Check for overshoot
		if (glm::abs(move) > glm::abs(diff)) {
			move = diff;
		}

		return value + move;
	}

	glm::vec2 move(glm::vec2 value, glm::vec2 target, float speed)
	{
		glm::vec2 diff = (target - value);
		// Check for too little distance
		if (glm::abs(diff.x) + glm::abs(diff.y) < 0.00001f) {
			return value;
		}
		glm::vec2 move = glm::normalize(diff) * speed * Time::getDeltaTime();

		// Check for overshoot
		if (glm::length(move) > glm::length(diff)) {
			move = diff;
		}

		return value + move;
	}

	float smoothMove(float value, float target, float speed, float maxVelocity)
	{
		float diff = (target - value);
		float move = diff * speed;

		// Check whether the computed speed stays under the maximum speed
		if (glm::abs(move) > maxVelocity) {
			// If it is, set it to the maximum velocity
			move = glm::sign(move) * maxVelocity;
		}

		return value + move * Time::getDeltaTime();
	}

	glm::vec2 smoothMove(glm::vec2 value, glm::vec2 target, float speed, float maxVelocity)
	{
		glm::vec2 diff = (target - value);
		glm::vec2 move = diff * speed;

		// Check whether the computed speed stays under the maximum speed
		if (glm::length(move) > maxVelocity) {
			// If it is, set it's length to the maximum velocity
			move = glm::normalize(move) * maxVelocity;
		}

		return value + move * Time::getDeltaTime();
	}

	float rotate(float value, float target, float speed)
	{
		// First we find the move direction
		float valueNormalized{ glm::mod(value, 360.0f) };
		float targetNormalized{ glm::mod(target, 360.0f) };

		if (valueNormalized < 0)
			valueNormalized += 360.0f;
		if (targetNormalized < 0)
			targetNormalized += 360.0f;

		float leftDiff{ targetNormalized - valueNormalized };
		float rightDiff{ valueNormalized - targetNormalized };

		if (leftDiff < 0)
			leftDiff += 360.0f;
		if (rightDiff < 0)
			rightDiff += 360.0f;

		// Rotate rightwards by default
		float moveDirection{ -1.0f };

		// Rotate leftwards
		if (leftDiff < rightDiff)
		{
			moveDirection = 1.0f;
		}

		float rotation{ moveDirection * speed * Time::getDeltaTime() };

		// If we would rotate more than there is space left, simply snap to the target
		if (glm::abs(rotation) >= glm::abs(glm::min(leftDiff, rightDiff)))
		{
			return target;
		}

		return value + rotation;
	}

	float smoothRotate(float value, float target, float speed, float maxVelocity)
	{
		// First we find the move direction
		float valueNormalized{ glm::mod(value, 360.0f) };
		float targetNormalized{ glm::mod(target, 360.0f) };

		if (valueNormalized < 0)
			valueNormalized += 360.0f;
		if (targetNormalized < 0)
			targetNormalized += 360.0f;

		float leftDiff{ targetNormalized - valueNormalized };
		float rightDiff{ valueNormalized - targetNormalized };

		if (leftDiff < 0)
			leftDiff += 360.0f;
		if (rightDiff < 0)
			rightDiff += 360.0f;

		// Rotate rightwards by default
		float moveDirection{ -1.0f };

		// Rotate leftwards
		if (leftDiff < rightDiff)
		{
			moveDirection = 1.0f;
		}

		float diff{ glm::min(leftDiff, rightDiff) };
		float rotation{ moveDirection * diff * speed };

		// Check whether the computed speed stays under the maximum speed
		if (glm::abs(rotation) > maxVelocity)
		{
			// If it is too fast, set it to the maximum velocity
			rotation = moveDirection * maxVelocity;
		}

		// Now for the overshoot check we will need the actual rotation,
		// so the current rotation is multiplied by delta time
		rotation = rotation * Time::getDeltaTime();
		// If we would rotate more than there is space left, simply snap to the target
		if (glm::abs(rotation) >= glm::abs(glm::min(leftDiff, rightDiff)))
		{
			return target;
		}

		return value + rotation;
	}
};
