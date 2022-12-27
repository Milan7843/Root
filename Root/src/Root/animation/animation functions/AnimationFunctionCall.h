#pragma once

class AnimationFunctionCall
{
public:
	~AnimationFunctionCall() {}

	virtual void call() {}

protected:
	AnimationFunctionCall() {}
};

