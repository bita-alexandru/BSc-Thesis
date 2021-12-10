#pragma once
#include <utility>

class Hashes
{
private:
	Hashes() {};
	~Hashes() {};
public:
	struct PairHash {
		template <class x, class y>
		std::size_t operator() (const std::pair<x, y>& p) const {
			size_t index = p.second * Sizes::TOTAL_CELLS + p.first;
			auto h = std::hash<x>{}(index);

			return h;
		}
	};
};

