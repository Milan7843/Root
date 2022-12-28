#pragma once

#include <memory>
#define AnimationFunctionCallPointer std::shared_ptr<AnimationFunctionCall> 

class AnimationFunctionCall
{
public:
	~AnimationFunctionCall() {}

	virtual void call() {}

protected:
	AnimationFunctionCall() {}
};

