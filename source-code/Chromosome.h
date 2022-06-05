#pragma once
#include <vector>

using namespace std;

class Chromosome
{
public:
	int id;

	vector<int> pattern;
	vector<int> initialPattern;

	int avgPopulation;
	int nOfGenerations;
	int initialSize;

	double fitness;

	unordered_map<int, string> cells;
	unordered_map<string, unordered_set<int>> statePositions;

	inline bool operator>(Chromosome& c)
	{
		return this->fitness > c.fitness;
	}

	inline bool operator<(Chromosome& c)
	{
		return this->fitness < c.fitness;
	}

	inline bool operator==(Chromosome& c)
	{
		return this->fitness == c.fitness;
	}
};

