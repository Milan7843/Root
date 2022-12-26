#pragma once

#include "Root/Logger.h"

#include <glm/gtc/constants.hpp>

#include <vector>
#include <algorithm>

#include <Root/Math.h>

/**
 * A struct that holds data for a gradient point.
 * 
 * point: the point at which evaluating the gradient will return the current value.
 * value: the value returned when evaluating the gradient at this point.
 */
template <class T>
struct GradientPoint
{
	float point;
	T value;
};

/**
 * A class for defining gradients.
 * 
 * Holds an array of 'GradientPoint's, 
 * where evaluating the gradient at the point of a specific GradientPoint will return its value, 
 * or an interpolated value if the evaluation value is not precisely on a GradientPoint.
 */
template <class T>
class Gradient
{
public:
	/**
	 * Create an empty gradient. Uses the default constructor of T to create one point.
	 */
	Gradient()
	{
		// Creating a single gradient point at 0, whose value will always be used.
		this->points = std::vector<GradientPoint<float>>({ 0.0f, T() });
	}
	/**
	 * Create an new gradient with just a single value which will always be returned when sampling.
	 *
	 * \param value: the single value.
	 */
	Gradient(T value)
	{
		// Creating a single gradient point at 0, whose value will always be used.
		this->points = std::vector<GradientPoint<T>>{ { 0.0f, value } };
	}

	/**
	 * Create a new gradient.
	 * 
	 * \param points a vector of gradient points.
	 */
	Gradient(std::vector<GradientPoint<T>> points)
		: points(points)
	{
		std::sort(points.begin(), points.end(), &Gradient::gradientSorter);
	}

	/**
	 * Create an new gradient with only a start point and an end point.
	 *
	 * \param startValue: the value to start on.
	 * \param endValue: the value to end on.
	 * \param duration: the total length of the gradient.
	 */
	Gradient(T startValue, T endValue, float duration)
	{
		// Creating one point at 0 and another at 'duration'
		this->points = std::vector<GradientPoint<T>>{ { 0.0f, startValue }, { duration, endValue } };
	}

	/**
	 * Create a gradient which can be used for sprite indexing.
	 * 
	 * \param startIndex: the index to start on (inclusive).
	 * \param endIndex: the index to end on (inclusive).
	 * \param duration: the total time this gradient has.
	 * \return a gradient which can be used for sprite indexing.
	 */
	static Gradient<unsigned int> spriteIndexGradient(unsigned int startIndex, unsigned int endIndex, float duration)
	{
		unsigned int frameCount = endIndex - startIndex + 1;

		std::vector<GradientPoint<unsigned int>> points = std::vector<GradientPoint<unsigned int>>{
			{ 0.0f, startIndex },
			{ duration * (1.0f - 1.0f / (float)frameCount), endIndex},
			{ duration, endIndex }
		};

		std::cout
			<< points[0].point << " -> " << points[0].value << "\n"
			<< points[1].point << " -> " << points[1].value << "\n"
			<< points[2].point << " -> " << points[2].value
			<< std::endl;

		Gradient<unsigned int> gradient(points);

		return gradient;
	}

	/**
	 * Create a gradient which can be used for 2D sprite indexing.
	 *
	 * \param startIndex: the 2D index to start on (inclusive).
	 * \param endIndex: the 2D index to end on (inclusive).
	 * \param duration: the total time this gradient has.
	 * \return a gradient which can be used for 2D sprite indexing.
	 */
	static Gradient<glm::ivec2> sprite2DIndexGradient(glm::ivec2 startIndex, glm::ivec2 endIndex, float duration)
	{
		unsigned int frameCount = std::max(endIndex.x - startIndex.x, endIndex.y - startIndex.y) + 1;

		std::vector<GradientPoint<glm::ivec2>> points = std::vector<GradientPoint<glm::ivec2>>{
			{ 0.0f, startIndex },
			{ duration * (1.0f - 1.0f / (float)frameCount), endIndex},
			{ duration, endIndex }
		};

		Gradient<glm::ivec2> gradient(points);

		return gradient;
	}

	~Gradient()
	{
		Logger::destructorMessage("Gradient");
	}

	/**
	 * Sample the gradient at a given point.
	 * 
	 * \param point the point at which to sample.
	 * \return the interpolated value at the given point.
	 */
	T sample(float samplePoint)
	{
		// If there are no points, return the value given by the default constructor of the type.
		if (points.size() == 0)
			return T();

		// If there is only one point, its value will always be returned.
		if (points.size() == 1)
			return points[0].value;

		unsigned int i{ 0 };
		for (GradientPoint<T>& gradientPoint : points)
		{
			// Found the right side point
			if (samplePoint < gradientPoint.point)
			{
				// If this is the first point, simply returns its value
				if (&gradientPoint == &points.front())
					return gradientPoint.value;

				// Otherwise find the left side and interpolate
				float t = Math::map(samplePoint, points[i-1].point, gradientPoint.point, 0.0f, 1.0f);
				return Math::cerp(points[i-1].value, gradientPoint.value, t);
			}

			i++;
		}

		// No right side was found: return the value of the right-most gradient point
		return points.back().value;
	}

private:

	std::vector<GradientPoint<T>> points;

	static bool gradientSorter(GradientPoint<T> const& lhs, GradientPoint<T> const& rhs)
	{
		return lhs.point < rhs.point;
	}
};
