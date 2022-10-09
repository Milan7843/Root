#include "Gradient.h"

template<class T>
Gradient<T>::Gradient(std::vector<GradientPoint<T>> points);

template<class T>
Gradient<T>::~Gradient()
{
	Logger::destructorMessage("Gradient");
}

template<class T>
T Gradient<T>::sample(float samplePoint)
{
	// If there are no points, return the value given by the default constructor of the type.
	if (points.size() == 0)
		return T();

	// If there is only one point, its value will always be returned.
	if (points.size() == 1)
		return points[0].value;

	unsigned int i{ 0 };
	for (GradientPoint& gradientPoint : points)
	{
		// Found the right side point
		if (samplePoint < gradientPoint.point)
		{
			// If this is the first point, simply returns its value
			if (&gradientPoint == &points.front())
				return gradientPoint.value;

			// Otherwise find the left side and interpolate
			float t = map(samplePoint, points[i].point, gradientPoint.point, 0.0f, 1.0f);
			return interpolate(points[i].value, gradientPoint.value, t);
		}

		i++;
	}

	// No right side was found: return the value of the right-most gradient point
	return points.back()->value;
}

template<class T>
bool gradientSorter(Gradient<T> const& lhs, Gradient<T> const& rhs)
{
	return lhs.point < rhs.point;
}

template<class T>
Gradient<T>::Gradient()
{
	// Creating a single gradient point at 0, whose value will always be used.
	this->points = std::vector<GradientPoint<float>>({ 0.0f, T() });
}

template<class T>
Gradient<T>::Gradient(T value)
{
	// Creating a single gradient point at 0, whose value will always be used.
	this->points = std::vector<GradientPoint<float>>({ 0.0f, value });
}

template<class T>
Gradient<T>::Gradient(std::vector<GradientPoint<T>> points)
	: points(points)
{
	std::sort(points.begin(), points.end(), &gradientSorter);
}