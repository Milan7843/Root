#pragma once

#include <Root/Math.h>
#include <Root/Time.h>

#include <map>

/**
 * Interpolated value is a table of values, which can be sampled.
 * Switching to a different value is smoothly interpolated.
 * 
 * TagType is the type of the identifying tag of a value.
 * ValueType is the type of the value.
 */
template <typename TagType, typename ValueType>
class InterpolatedValue
{
public:

	/**
	 * Create a new interpolated value.
	 * 
	 * \param transitionDuration: the time a transition to a new value takes.
	 */
	InterpolatedValue(float transitionDuration = 0.1f);

	~InterpolatedValue() {}

	/**
	 * Add a new value to the table.
	 * 
	 * \param tag: the tag to identify the value with.
	 * \param value: the value corresponding to the given tag.
	 */
	void addValue(TagType tag, ValueType value);

	/**
	 * Set the value that is currently active.
	 *
	 * \param tag: the tag identifying the active value.
	 */
	void setActiveValue(TagType tag);

	/**
	 * Sample this interpolated value.
	 * 
	 * \returns the current active value interpolated from the last active value.
	 */
	ValueType sample();

	/**
	 * Set the transition duration for scale and color transition.
	 *
	 * \param transitionDuration: the new transition duration.
	 */
	void setTransitionDuration(float transitionDuration);

private:

	std::map<TagType, ValueType> table;

	bool activeValueSet{ false };
	ValueType activeValue;
	TagType activeTag;
	ValueType previousValue;

	float timeOfValueChange;

	float transitionDuration;
};

template <typename TagType, typename ValueType>
InterpolatedValue<TagType, ValueType>::InterpolatedValue(float transitionDuration)
	: transitionDuration(transitionDuration)
	, timeOfValueChange(Time::getTime())
{
}

template <typename TagType, typename ValueType>
void InterpolatedValue<TagType, ValueType>::addValue(TagType tag, ValueType value)
{
	// The first added value will be active by default
	if (!activeValueSet)
	{
		activeValue = value;
		previousValue = value;
		activeTag = tag;
		activeValueSet = true;
	}

	// Insert or overwrite
	table.insert_or_assign(tag, value);
}

template <typename TagType, typename ValueType>
void InterpolatedValue<TagType, ValueType>::setActiveValue(TagType tag)
{
	// Don't change anything if we are already at the active value
	if (activeTag == tag)
	{
		return;
	}

	typename std::map<TagType, ValueType>::iterator iterator = table.find(tag);

	// Found the tag
	if (iterator != table.end())
	{
		// Swapping to the new value
		//previousValue = sample();
		previousValue = activeValue;
		activeValue = iterator->second;
		activeTag = tag;

		// And marking the time for interpolation
		timeOfValueChange = Time::getTime();
	}

	// Did not find the character
}

template <typename TagType, typename ValueType>
ValueType InterpolatedValue<TagType, ValueType>::sample()
{
	// Check if any values are set
	if (!activeValueSet)
	{
		Logger::logError("Cannot sample InterpolatedValue without any values.");
		return ValueType{};
	}

	float timeSinceValueChange{ Time::getTime() - timeOfValueChange };

	// Already past the transition
	if (timeSinceValueChange >= transitionDuration)
		return activeValue;

	// Otherwise interpolate
	float t{ timeSinceValueChange / transitionDuration };
	return Math::cerp<ValueType>(previousValue, activeValue, t);
}

template <typename TagType, typename ValueType>
void InterpolatedValue<TagType, ValueType>::setTransitionDuration(float transitionDuration)
{
	this->transitionDuration = transitionDuration;
}
