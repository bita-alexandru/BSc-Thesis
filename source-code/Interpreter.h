#pragma once
#include "Transition.h"

#include <unordered_set>

using namespace std;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	vector<pair<int, string>> Process(string& rules);
	vector<pair<string, Transition>>& GetTransitions();
	
	void SetStates(unordered_map<string, string>& states);
	void SetNeighbors(unordered_set<string>& neighbors);
private:
	vector<pair<string, Transition>> m_Transitions;
	unordered_map<string, string> m_States;
	unordered_set<string> m_Neighbors;

	vector<string> AND = { ",", "&", "AND" };
	vector<string> OR = { "|", "OR" };

	string LEFT_SPACED = "";
	string RIGHT_SPACED = "";
	string BOTH_SPACED = "/:()[]@=,&|;!";
	string NON_SPACED = "+-#";
	int spaces = 0;

	bool FindWord(int& cursor, string& rules, string& s, bool comment = false);
	bool NextTransition(int& cursor, string& rules, stringstream& ss);
	bool CheckState(string& state);
	bool UpdateChars(int& chars, string& s);
	bool UpdateSize(int& size);
	void MarkInvalid(bool& valid, vector<pair<int, string>>& invalid, string reason, int& cursor);
	bool CheckDirection(string& direction);
	int CheckNumber(string& number, Transition& transition, int& count);
	bool SkipIfComment(int& cursor, string& rules, stringstream& ss, string& s);
};

