#pragma once

/**
* Class for custom scripts.
* To use, make a subclass of Script which overrides the start() and update() functions.
* The start function will be called once, and the update function every frame.
*/
class Script
{
public:

	/**
	 * The start function of this script, gets called automatically at the start of the game.
	 * May be implemented by a subclass, but this is not required.
	 */
	virtual void start() {}

	/**
	 * The update function of this script, gets called automatically every frame.
	 * May be implemented by a subclass, but this is not required.
	 */
	virtual void update() {}

	bool internal_started();

	void internal_start();

private:

	// Holds whether the start method has been called or not
	bool internal_called_start{ false };
};
