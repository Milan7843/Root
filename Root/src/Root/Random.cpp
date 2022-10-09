#include "Random.h"

namespace Random
{
	void setSeed(unsigned int seed)
	{
		srand(seed);
	}

	float between(float min, float max)
	{
		return min + ((float)rand() / (float)RAND_MAX) * (max - min);
	}
};
