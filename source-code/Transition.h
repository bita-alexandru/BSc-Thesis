#pragma once
#include <unordered_map>
#include <vector>
#include <string>

#define CONDITIONS_AND vector<pair<int, string>> // pair = (<number of cells>, <state name>)
#define CONDITIONS_OR vector<CONDITIONS_AND>
#define RULES_AND vector<pair<string, CONDITIONS_OR>> // pair = (<neighborhood>, ...)
#define RULES_OR vector<RULES_AND>

using namespace std;

struct Transition
{
	string state;
	
	CONDITIONS_AND andConditions;
	CONDITIONS_OR orConditions;

	RULES_AND andRules;
	RULES_OR orRules;
};
