#pragma once
class Random {
public:
	static void Randomize();
	static int GetNext(int min, int max);
	template<typename T> static void Shuffle(T first, T last);
};


#include <algorithm>
#include <random>

template<typename T> void Random::Shuffle(T first, T last)
{
	std::random_device rd;
	std::shuffle(first, last, std::default_random_engine(rd()));
}
