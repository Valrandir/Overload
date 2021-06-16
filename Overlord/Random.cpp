#include <stdlib.h>
#include <time.h>
#include "Random.hpp"

void Random::Randomize()
{
	srand((unsigned int)time(NULL));
}

int Random::GetNext(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}
