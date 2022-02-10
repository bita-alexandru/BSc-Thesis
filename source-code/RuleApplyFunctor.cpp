#include "RuleApplyFunctor.h"

#include "wx/log.h"

RuleApplyFunctor::RuleApplyFunctor
(
	string _type,
	string _state,
	pair<string, Transition> _rule,
	unordered_map<string, string>* _states,
	unordered_set<string>* _neighbors,
	unordered_map<pair<int, int>, pair<string, wxColour>, Hashes::PairInt>* _cells,
	unordered_map<string, unordered_set<pair<int, int>, Hashes::PairInt>>* _statePositions,
	bool* _forceClose
)
{
	type = _type;
	state = _state;
	rule = _rule;
	states = _states;
	neighbors = _neighbors;
	cells = _cells;
	statePositions = _statePositions;
	forceClose = _forceClose;
}

RuleApplyFunctor::~RuleApplyFunctor()
{
	Clear();
}

void RuleApplyFunctor::operator()(int start, int size)
{
	ApplyRule(start, size);

	Clear();
}

vector<pair<int, int>> RuleApplyFunctor::GetApplied()
{
	if (forceClose && *forceClose == true) return {};
	return applied;
}

string RuleApplyFunctor::GetState(int x, int y)
{
	if (*forceClose == true) return "FREE";

	if (cells->find({ x,y }) == cells->end()) return "FREE";

	return cells->at({x, y}).first;
}

bool RuleApplyFunctor::InBounds(int x, int y)
{
	return (x >= 0 && x < Sizes::N_COLS&& y >= 0 && y < Sizes::N_ROWS);
}

bool RuleApplyFunctor::ApplyOnCell(int x, int y, Transition& rule)
{
	std::unordered_map<std::string, std::string> neighborhood = GetNeighborhood(x, y);

	//wxLogDebug("[CELL_NEIGHBORHOOD]");
	//for (auto& it : neighborhood) wxLogDebug("<%s>=%s", it.first, it.second);

	bool ruleValid = true;
	// iterate through the chain of "OR" rules
	for (auto& rulesOr : rule.orRules)
	{
		ruleValid = true;
		//wxLogDebug("[RULES_OR]");

		// iterate through the chain of "AND" rules
		for (auto& rulesAnd : rulesOr)
		{
			//wxLogDebug("[RULES_AND]");
			std::vector<std::string> ruleNeighborhood = rulesAnd.first;
			//wxLogDebug("[RULE_NEIGHBORHOOD]");
			//for (auto& it : ruleNeighborhood)wxLogDebug("<%s>", it);

			bool conditionValid = true;
			// iterate through the chain of "OR" conditions
			for (auto& conditionsOr : rulesAnd.second)
			{
				conditionValid = true;

				// iterate through the chain of "AND" conditions
				for (auto& conditionsAnd : conditionsOr)
				{
					std::string conditionState = conditionsAnd.second;
					//wxLogDebug("CONDITION_STATE=%s", conditionState);

					int occurences = 0;
					if (ruleNeighborhood[0] == "ALL")
					{
						for (auto& neighbor : neighborhood)
							if (neighbor.second == conditionState) occurences++;
					}
					else for (auto& neighbor : ruleNeighborhood)
					{
						if (neighbors->find(neighbor) != neighbors->end())
						{
							if (neighborhood[neighbor] == conditionState) occurences++;
						}
						// otherwise, maybe throw error; to do: decide (line 1434)
					}

					//wxLogDebug("OCCURENCES=%i", occurences);

					int conditionNumber = conditionsAnd.first.first;
					int conditionType = conditionsAnd.first.second;

					//wxLogDebug("CONDITION_NUMBER=%i, CONDITION_TYPE=%i", conditionNumber, conditionType);

					switch (conditionType)
					{
					case TYPE_EQUAL:
						if (occurences != conditionNumber) conditionValid = false;
						break;
					case TYPE_LESS:
						if (occurences >= conditionNumber) conditionValid = false;
						break;
					case TYPE_MORE:
						if (occurences <= conditionNumber) conditionValid = false;
						break;
					default:
						break;
					}
				}

				if (conditionValid) break;
			}

			if (!conditionValid)
			{
				ruleValid = false;
				break;
			}
		}

		if (ruleValid) break;
	}

	return ruleValid;
}

unordered_map<string, string> RuleApplyFunctor::GetNeighborhood(int x, int y)
{
	std::unordered_map<std::string, std::string> neighborhood;

	if (*forceClose == true) return neighborhood;

	std::unordered_map<std::string, std::pair<int, int>> dxy(
		{
			{ "NW",{-1,-1} }, { "N",{0,-1} }, { "NE",{1,-1} },
			{ "W",{-1,0} }, { "C",{0,0} }, { "E",{1,0} },
			{ "SW",{-1,1} }, { "S",{0,1} }, { "SE",{1,1} },
		}
	);

	// iterate through neighbors and mark the state they have
	for (auto& neighbor : *neighbors)
	{
		std::pair<int, int> d = dxy[neighbor];

		int nx = x + d.first;
		int ny = y + d.second;

		if (InBounds(nx, ny)) neighborhood.insert({ neighbor,GetState(nx,ny) });
	}

	return neighborhood;
}

void RuleApplyFunctor::Clear()
{
	states = nullptr;
	neighbors = nullptr;
	cells = nullptr;
	statePositions = nullptr;
	forceClose = nullptr;
}

void RuleApplyFunctor::ApplyRule(int start, int size)
{
	if (type == "FREE_ALL")
	{
		for (int i = start; i < start + size && *forceClose == false; i++)
		{
			int x = i % Sizes::N_COLS;
			int y = i / Sizes::N_COLS;

			if (GetState(x, y) == "FREE" && ApplyOnCell(x, y, rule.second)) applied.push_back({ x,y });
		}

		return;
	}

	if (type == "STATE_ALL")
	{
		auto i = statePositions->at(state).begin();
		while (start-- && *forceClose == false) i++;

		while (size-- && *forceClose == false)
		{
			int x = i->first;
			int y = i->second;

			if (ApplyOnCell(x, y, rule.second)) applied.push_back({ x,y });

			i++;
		}

		return;
	}

	if (type == "ADJACENT")
	{
		auto i = statePositions->at(state).begin();
		while (start-- && *forceClose == false) i++;

		int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
		int dy[8] = { -1,-1,0,1,1,1,0,-1 };

		while (size-- && *forceClose == false)
		{
			int x = i->first;
			int y = i->second;

			for (int d = 0; d < 8; d++)
			{
				int nx = x + dx[d];
				int ny = y + dy[d];

				if (InBounds(nx, ny) && GetState(nx, ny) == rule.first && ApplyOnCell(nx, ny, rule.second)) applied.push_back({ nx,ny });
			}

			i++;
		}

		return;
	}
}
