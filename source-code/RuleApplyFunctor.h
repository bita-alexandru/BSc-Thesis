#pragma once
#include "Sizes.h"
#include "Hashes.h"
#include "Transition.h"

#include "wx/colour.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class RuleApplyFunctor
{
public:
	RuleApplyFunctor(
		string _type,
		string _state,
		pair<string, Transition> _rule,
		unordered_map<string, string>* _states,
		unordered_set<string>* _neighbors,
		unordered_map<pair<int, int>, pair<string, wxColour>, Hashes::PairInt>* _cells,
		unordered_map<string, unordered_set<pair<int, int>, Hashes::PairInt>>* _statePositions
		)
		;
	~RuleApplyFunctor();

	void operator() (int start, int size);

	vector<pair<int, int>> GetApplied();
private:
	vector<pair<int, int>> applied;

	string type;
	string state;
	pair<string, Transition> rule;
	unordered_map<string, string>* states = nullptr;
	unordered_set<string>* neighbors = nullptr;
	unordered_map<pair<int, int>, pair<string, wxColour>, Hashes::PairInt>* cells = nullptr;
	unordered_map<string, unordered_set<pair<int, int>, Hashes::PairInt>>* statePositions = nullptr;

	string GetState(int x, int y);
	bool InBounds(int x, int y);
	bool ApplyOnCell(int x, int y, Transition& rule);
	unordered_map<string, string> GetNeighborhood(int x, int y);

	void Clear();
	void ApplyRule(int start, int size);
};

