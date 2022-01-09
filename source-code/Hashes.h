#pragma once
#include "Sizes.h"

#include <utility>

using namespace std;

class Hashes
{
private:
	Hashes() {};
	~Hashes() {};
public:
	struct PairInt {
		inline size_t operator() (const pair<int, int>& p) const {
			return p.second * Sizes::TOTAL_CELLS + p.first;
		}
	};
};

